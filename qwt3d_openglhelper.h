#ifndef QWT3D_OPENGLHELPER_H
#define QWT3D_OPENGLHELPER_H

#include "GL/glu.h"
#include "qglobal.h"
#include <QtOpenGL/qgl.h>

namespace Qwt3D {

class GLStateBewarer{
public:
    GLStateBewarer(GLenum what, bool on, bool persist=false){
        state_ = what;
        stateval_ = glIsEnabled(what);
        if (on){
            turnOn(persist);
        }
        else {
            turnOff(persist);
        }
    }
    ~GLStateBewarer(){
        if (stateval_){
            glEnable(state_);
        }
        else {
            glDisable(state_);
        }
    }

    void turnOn(bool persist = false){
        glEnable(state_);
        if (persist){
            stateval_ = true;
        }
    }

    void turnOff(bool persist = false){
        glDisable(state_);
        if (persist){
            stateval_ = false;
        }
    }

private:
    GLenum state_;
    bool stateval_;
};

inline const GLuByte * gl_error(){
    GLenum errcode;
    const GLubyte *err = 0;
    if ((errcode == glGetError()) != GL_NO_ERROR){
        err = gluErrorString(errcode);
    }
    return err;
}

inline void saveGLDeleteLists(GLuint& lstidx, GLsizei range){
    if (glIsList(lstidx)){
        glDeleteLists(lstidx, range);
    }
    lstidx = 0;
}

inline void getMatrices(GLdouble *modelMatrix, GLdouble *projMatrix, GLint *viewport){
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
}

inline bool viewPort2World(double &objx, double &objy, double &objz, double winx, double winy, double winz){
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];

    getMatrices(modelMatrix, projMatrix, viewport);
    int res = gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

    return res != GL_FALSE;
}

inline bool world2ViewPort(double& winx, double &winy, double &winz, double objx, double objy, double objz){
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];

    getMatrices(modelMatrix, projMatrix, viewport);
    int res = gluProject(objx, objy, objz, modelMatrix, projMatrix, viewport, &winx, &winy, &winz);

    return res != GL_FALSE;
}
}

#endif // QWT3D_OPENGLHELPER_H
