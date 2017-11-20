#ifndef QGLESWIDGET_H
#define QGLESWIDGET_H

#include <QWidget>
#include <FreeImage.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include "tool/CELLMath.hpp"
#include "tool/program_p2_c4.h"
#include "tool/mycamera.h"
#define MESA_EGL_NO_X11_HEADERS
#include <EGL/egl.h>
#include <GLES2/gl2.h>


enum KEYMODE
{
    KEY_NULL,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_W,
};

enum MOUSEMODE
{
    MOUSE_NULL,
    MOUSE_LEFTDOWN,
    MOUSE_RIGHTDOWN,
    MOUSE_LEFTUP,
    MOUSE_RIGHTUP,
    MOUSE_WHEEL,
};

//typedef struct
//{
//    CELL::float2 pos;
//    CELL::float2 uv;
//    CELL::Rgba4Byte color;
//}Vertex;

typedef struct
{
    float x,y,z;
    float nx,ny,nz;
    float u,v;
}Vertex;

typedef struct
{
    float x,y,z;
    float nx,ny,nz;
}V3N3;

typedef struct
{
    float x,y,z;
}LINNES;

class QGLESWIDGET : public QWidget
{
    Q_OBJECT
public:
    explicit QGLESWIDGET(QWidget *parent = 0);
    ~QGLESWIDGET();

    //! 窗口的高度
    int         _width;
    //! 窗口的宽度
    int         _height;
    /// for gles2.0
    EGLConfig   _config;
    EGLSurface  _surface;
    EGLContext  _context;
    EGLDisplay  _display;

    //! 增加shader
    PROGRAM_P2_C4   _shader;
    unsigned int    _texture;
    unsigned int    _texture1;
    unsigned int    _textureAuto;
    MYCAMERA        _camera;

    //!缓冲区<帧>
    unsigned int    _frameBO;
    unsigned int    _renderBO;


    bool init_QGW(std::vector<QString> fileName);

    //! 初始化 OpenGLES2.0
    bool    initOpenGLES20();
    //! 销毁OpenGLES2.0
    void    destroyOpenGLES20();
    virtual void render();
    virtual unsigned int loadTexture(const char* fileName);
    virtual unsigned int loadTextureMipmap(std::vector<QString> fName);
    virtual unsigned int createTexture(int width,int height);

    void drawImage();

    void displayGround();
    void displayRect();

    QObject *parent;
    //!实现键盘事件
    bool eventFilter( QObject * target , QEvent * event );
    void keyPressEvent(QKeyEvent * e);

    //!鼠标事件
    CELL::float2 pos;
    MOUSEMODE typeMouse;
    bool leftB;
    bool rightB;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

signals:
    void sendKeyEvent(KEYMODE key);
    void sendMouseEvent(MOUSEMODE mouse,CELL::int2 pstart,CELL::int2 pend);
    void sendWheelEvent(MOUSEMODE mouse,int p,CELL::int2 pos);
public slots:
};

#endif // QGLESWIDGET_H
