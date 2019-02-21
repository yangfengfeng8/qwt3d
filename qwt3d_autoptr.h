#ifndef QWT3D_AUTOPTR_H
#define QWT3D_AUTOPTR_H

namespace Qwt3D {

template <typename T>
class qwt3d_ptr{
public:
    explicit qwt3d_ptr(T *ptr = 0) : rawptr_(ptr){

    }
    ~qwt3d_ptr(){
        destroyRawPtr();
    }

    qwt3d_ptr(qwt3d_ptr const& val){
        rawptr_ = val.rawptr_->clone();
    }

    qwt3d_ptr<T>& operator=(qwt3d_ptr const& val){
        if (this == &val){
            return *this;
        }
        destroyRawPtr();
        rawptr_ = val.rawptr_->clone();

        return *this;
    }

    T* operator->() const{
        return rawptr_;
    }

    T& operator*() const{
        return *rawptr_;
    }

private:
    T *rawptr_;

    void destroyRawPtr(){
        if (rawptr_){
            rawptr_->destroy();
        }
        rawptr_ = 0;
    }
};

}

#endif // QWT3D_AUTOPTR_H
