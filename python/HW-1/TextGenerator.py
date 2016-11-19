"""Text generation module.
This module contains the text generator. The generator can automatically
generate logical text after learning on the given sample text. The method
of the text generation bases on the Markov chains:
https://en.wikipedia.org/wiki/Markov_chain#Markov_text_generators

The program has 4 working modes:
tokenize : Split input text into tokens.
probabilities : Count probabilities of chains in input text.
generate : Generate text.
test : Run unit tests.

"""
import sys
import codecs
import unittest
import random
import math
import argparse
from collections import defaultdict, deque

#Define the encoding of sample and generated texts
ENCODING = "utf8"

class TextGenerator:
    """Class for text generation.

    Attributes:
        input_text (iterable): Sample text for learning.
        depth (int): Maximal depth of Markov chains
        size (int): Approximate number of words into the generated text
    """
    def __init__(self, input_text, depth=1, size=100):
        self.input_text = input_text
        self.depth = depth
        self.size = size

    def tokenize_string(self, input_string):
        """ Split the input_string into tokens.

        Args:
            input_string (str): String for tokenizing.
            Token - the sequence of characters that entirely consists of
            letters or digits; distinct character.

        Returns:
            list: List of tokens
        """
        tokens = []
        START = -1
        ALPHABET_TOKEN = 0
        DIGIT_TOKEN = 1
        current_token = ''
        current_state = START
        for symbol in input_string:
            if symbol.isalpha():
                if current_state == ALPHABET_TOKEN or current_state == START:
                    current_state = ALPHABET_TOKEN
                else:
                    tokens.append(current_token)
                    current_token = ''
                    current_state = ALPHABET_TOKEN
                current_token += symbol
            elif symbol.isdigit():
                if current_state == DIGIT_TOKEN or current_state == START:
                    current_state = DIGIT_TOKEN
                else:
                    tokens.append(current_token)
                    current_token = ''
                    current_state = DIGIT_TOKEN
                current_token += symbol
            else:
                if current_state != START:
                    tokens.append(current_token)
                    current_token = ''
                tokens.append(symbol)
                current_state = START
        if current_state != START:
            tokens.append(current_token)
        return tokens

    def tokenize(self):
        """Tokenize the input_text into tokens.

        Returns:
            list: List of tokens.
        """
        tokens = []
        for line in self.input_text:
            tokens += self.tokenize_string(line)
        return tokens

    def convert_deque_to_tuple(self, deque):
        """Convert the deque to the tuple.
        This method is needed for using the deque as the dictionary key.

        Args:
            deque (deque): Input deque.

        Returns:
            tuple: The tuple with contents of the deque
        """
        return tuple(list(deque))

    def calculate_frequencies(self, token_selector):
        """Calculate the frequency of the word's entry after each chain.

        Args:
            token_selector (function): The function for selecting necessary tokens.
            This function is necessary because in 'probabilities' only letter tokens
            are required but in 'generation' mode it is better to use all tokens to get
            more logical text.

        Returns:
            dict: Dictionary with chains as the keys and dictionaries of words that
            occur after these chains with frequencies of its entries as values.
        """
        frequencies = defaultdict(lambda: defaultdict(int))
        for line in self.input_text:
            current_history = deque(maxlen=self.depth)
            for token in self.tokenize_string(line):
                if token_selector(token):
                    for history_length in xrange(len(current_history)+1):
                        chain = list(current_history)[history_length:]
                        frequencies[tuple(chain)][token] += 1
                    current_history.append(token)
        return frequencies

    def calculate_probabilities(self, token_selector):
        """Calculate the probabilities of the word's entry after each chain
        Calculate probabilities by division corresponding frequencies on
        total number of chain's entries.

        Args:
            token_selector (function): The function for selecting necessary tokens.
            This function is necessary because in 'probabilities' only letter tokens
            are required but in 'generation' mode it is better to use all tokens to get
            more logical text.

        Returns:
            dict: Dictionary with chains as the keys and dictionaries of words that
            occur after these chains with probabilities of its entries as values.
        """
        frequencies = self.calculate_frequencies(token_selector)
        probabilities = defaultdict(lambda: defaultdict(float))
        for history, words_counters in frequencies.items():
            total_words_number = sum(words_counters.values())
            for word, counter in words_counters.items():
                probabilities[history][word] = float(counter) / total_words_number
        return probabilities

    def choose_random_word(self, probabilities):
        """Choose the random from specified word's probabilities distribution.
        The word is choosing by generation the random point in the range [0.0; 1.0)
        and choosing the interval of probabilities that contains this point.

        Args:
            probabilities (dict): Distribution word's probabilities. Words are the keys
            probabilities are the values in the dictionary.

        Returns:
            str: Random word from the keys of input dictionary
        """
        cumulative_probability = 0.0
        random_point = random.random()
        for key, value in probabilities.items():
            if (random_point >= cumulative_probability and
                    random_point <= cumulative_probability + value):
                return key
            cumulative_probability += value

    def generate_text_tokens(self):
        """Generate tokens of the new text.
        Tokens are generated by choosing random word that can occur after
        current chain. After each new word generation it is added to the
        end of current chain. If the length of chain becomes greater than
        depth attribute value, then the first token of the chain is deleted.

        Returns:
            list: The list of generated tokens.
        """
        probabilities = self.calculate_probabilities(lambda x: True)
        tokens = []
        history = deque(maxlen=self.depth)
        history.append('.')
        while len(tokens) < self.size:
            history_tuple = self.convert_deque_to_tuple(history)
            while len(history) > 0 and len(probabilities[history_tuple]) == 0:
                history.popleft()
            history_tuple = self.convert_deque_to_tuple(history)
            new_token = self.choose_random_word(probabilities[history_tuple])
            tokens.append(new_token)
            history.append(new_token)
        return tokens

    def generate_text(self):
        """Generate the logical text.
        Generate tokens and join them into line with the maximum
        length = 50.

        Returns:
            strL Generated text

        """
        tokens = self.generate_text_tokens()
        text = []
        line_words = []
        number_words_in_line = 50
        for i, token in enumerate(tokens):
            line_words.append(token)
            if len(line_words) > number_words_in_line:
                if i + 1 < len(tokens) and tokens[i + 1].isalnum():
                        text.append((''.join(line_words)).strip())
                        line_words = []
        text.append((''.join(line_words)).strip())
        return '\n'.join(text)


class TestTextGenerator(unittest.TestCase):
    """Class for testing the main functionality of the TextGenerator class"""
    def test_tokenize(self):
        """Test of strings tokenizing."""
        generator = TextGenerator([])
        self.assertEqual(generator.tokenize_string('abc, aaa'), ['abc', ',', ' ', 'aaa'])
        self.assertEqual(generator.tokenize_string('abc, a3aa'), ['abc', ',', ' ', 'a', '3', 'aa'])
        self.assertEqual(generator.tokenize_string(' '), [' '])
        self.assertEqual(generator.tokenize_string(''), [])

    def test_calculate_probabilities(self):
        """Test of probabilities calculation.
        The sample text is defined as the list of strings.
        """
        generator = TextGenerator(['a a b', 'c a b'])
        probabilities = generator.calculate_probabilities(lambda x: x.isalpha())
        self.assertEqual(probabilities[tuple()]['a'], 3.0 / 6.0)
        self.assertEqual(probabilities[tuple()]['b'], 2.0 / 6.0)
        self.assertEqual(probabilities[tuple()]['c'], 1.0 / 6.0)
        self.assertEqual(probabilities[tuple(['a'])]['a'], 1.0 / 3.0)
        self.assertEqual(probabilities[tuple(['a'])]['b'], 2.0 / 3.0)
        self.assertEqual(probabilities[tuple(['c'])]['a'], 1.0)

    def test_choose_random_word(self):
        """Test of choosing the random word. """
        generator = TextGenerator([])
        words = {'a': 0.2, 'b': 0.3, 'c': 0.5}
        word = generator.choose_random_word(words)
        self.assertTrue(word in words.keys())

    def test_generate_text(self):
        """Test of text generation.
        The sample text is defined as the list of strings.
        """
        generator = TextGenerator(['a a b', 'c a b', 'a a b', 'c a b'])
        text = generator.generate_text()
        self.assertTrue(len(text) >= 0)


#Functions for reading input data
def read_arguments():
    """Read the input arguments string.

    Returns:
        str: Input arguments string
    """
    return raw_input()

def read_text_line():
    """Generator for effective reading the sample text.

    Returns:
        str: Current line of the sample text.
    """
    for line in codecs.getreader(ENCODING)(sys.stdin):
        line = line.rstrip('\n')
        yield line


#Printing functions. Use the encoding specified by the constant ENCODE
def print_tokens(tokens):
    """Print tokens."""
    writer = codecs.getwriter(ENCODING)(sys.stdout)
    writer.write('\n'.join(tokens))

def print_probabilities(probabilities):
    """Print the probabilities in required format"""
    writer = codecs.getwriter(ENCODING)(sys.stdout)
    for history, word_prob in sorted(probabilities.items(), key=lambda x: ' '.join(x[0])):
        writer.write(' '.join(history) + '\n')
        for word, prob in sorted(word_prob.items()):
            writer.write(u"  {word}: {prob:.2f}\n".format(word=word, prob=prob))

def print_generated_text(generated_text):
    """Print generated text"""
    writer = codecs.getwriter(ENCODING)(sys.stdout)
    writer.write(generated_text)

#Functions corresponding modes of the program
def tokenize(args):
    """Implement the tokenize mode"""
    generator = TextGenerator(read_text_line())
    tokens = generator.tokenize()
    print_tokens(tokens)

def count_probabilities(args):
    """Implement the probabilities mode"""
    generator = TextGenerator(read_text_line(), args.depth)
    probabilities = generator.calculate_probabilities(lambda x: x.isalpha())
    print_probabilities(probabilities)

def generate(args):
    """Implements the generate mode"""
    generator = TextGenerator(read_text_line(), args.depth, args.size)
    generated_text = generator.generate_text()
    print_generated_text(generated_text)

def test(args):
    """Implements the test mode"""
    unittest.main()

#parsing the input arguments
def main():
    argument_string = read_arguments()
    parser = argparse.ArgumentParser(description='Generate texts.')
    subparsers = parser.add_subparsers()
    #create parser for tokenize command
    parser_tokenize = subparsers.add_parser('tokenize',
                                            help='split input text into tokens')
    parser_tokenize.set_defaults(func=tokenize)
    #create parser for probabilities command
    parser_probabilities = subparsers.add_parser(
        'probabilities',
        help='count probabilities of chains in input text')
    parser_probabilities.add_argument('--depth', type=int,
                                      help='maximal length of chain',
                                      required=True)
    parser_probabilities.set_defaults(func=count_probabilities)
    #create parser for generate command
    parser_generate = subparsers.add_parser('generate',
                                            help='generate text')
    parser_generate.add_argument('--depth', type=int,
                                 help='maximal length of chain',
                                 required=True)
    parser_generate.add_argument('--size', type=int,
                                 help='approximate number of words in the generated text',
                                 required=True)
    parser_generate.set_defaults(func=generate)
    #create parser for test command
    parser_test = subparsers.add_parser('test',
                                        help='run unit tests')
    parser_test.set_defaults(func=test)

    #parse arguments
    args = parser.parse_args(argument_string.split())
    args.func(args)

main()
