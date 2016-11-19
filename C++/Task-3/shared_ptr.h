#include <algorithm>

namespace smart_pointers
{
    template <class T>
    struct DefaultDeleter
    {
        void operator()(T *object)
        {
            delete object;
        }
    };

    template <class T>
    struct ArrayDeleter
    {
        void operator()(T *object)
        {
            delete[] object;
        }
    };

    template<class ObjectType, class DeleterType = DefaultDeleter<ObjectType>>
    class ReferenceCounter
    {
    private:

        size_t reference_counter_;
        DeleterType deleter_;
        ObjectType *managed_object_;

        void Destroy()
        {
            deleter_(managed_object_);
        }

    public:

        ReferenceCounter() : reference_counter_(1) {};

        ReferenceCounter(ObjectType *object) :
            reference_counter_(1),
            managed_object_(object) {}

        ReferenceCounter(ObjectType *object, DeleterType deleter) :
            reference_counter_(1),
            managed_object_(object),
            deleter_(deleter) {};

        void Increase()
        {
            reference_counter_++;
        }

        void Decrease()
        {
            reference_counter_--;
            if (reference_counter_ == 0)
            {
                Destroy();
                delete this;
            }
        }

        bool operator == (int value)
        {
            return reference_counter_ == value;
        }

        int GetReferenceNumber() const
        {
            return reference_counter_;
        }
    };

    template<class T, class DeleterType = DefaultDeleter<T>>
    class shared_ptr
    {
    public:
        shared_ptr() :
            managed_object_(nullptr) 
        {
            reference_counter_ = new ReferenceCounter<T, DeleterType>(nullptr);
        }

        shared_ptr(std::nullptr_t) : shared_ptr() {}

        template <class ObjectType>
        explicit shared_ptr(ObjectType * object) :
            managed_object_(object) 
        {
            reference_counter_ = new ReferenceCounter<ObjectType>(object);
        }

        template <class ObjectType, class DeleterType>
        shared_ptr(ObjectType * object, DeleterType deleter) :
            managed_object_(object)
        {
            reference_counter_ = new ReferenceCounter<ObjectType, DeleterType>(object, deleter);
        }

        template <class DeleterType>
        shared_ptr(std::nullptr_t object, DeleterType deleter) :
            managed_object_(object) 
        {
            reference_counter_ = new ReferenceCounter<nullptr_t, DeleterType>(T*(0), deleter);
        }

        shared_ptr(const shared_ptr& another)
        {
            if (another.use_count() == 0)
            {
                managed_object_ = nullptr;
                reference_counter_ = new ReferenceCounter<T>(nullptr);
            }
            else
            {
                managed_object_ = another.managed_object_;
                reference_counter_ = another.reference_counter_;
                reference_counter_->Increase();
            }
        }

        shared_ptr& operator = (const shared_ptr& another)
        {
            if (reference_counter_ != another.reference_counter_)
            {
                reference_counter_->Decrease();
                reference_counter_ = another.reference_counter_;
                reference_counter_->Increase();
                managed_object_ = another.managed_object_;
            }
            return *this;
        }

        void swap(shared_ptr& another)
        {
            std::swap(reference_counter_, another.reference_counter_);
            std::swap(managed_object_, another.managed_object_);
        }

        void reset()
        {
            reference_counter_->Decrease();
            reference_counter_ = new ReferenceCounter<T>(nullptr);
            managed_object_ = nullptr;
        }

        T* get() const
        {
            return managed_object_;
        }

        T& operator*() const
        {
            return *managed_object_;
        }

        T* operator->() const
        {
            return managed_object_;
        }

        long int use_count() const
        {
            if (managed_object_ == nullptr)
            {
                return 0;
            }
            return reference_counter_->GetReferenceNumber();
        }

        bool unique() const
        {
            return use_count() == 1;
        }

        explicit operator bool() const
        {
            return managed_object_ != nullptr;
        }

        ~shared_ptr()
        {
            reference_counter_->Decrease();
        }

    private:
        ReferenceCounter<T, DeleterType> *reference_counter_;
        T * managed_object_;
    };
};
