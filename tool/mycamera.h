#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QObject>
#include <tool/CELLMath.hpp>

class MYCAMERA : public QObject
{
    Q_OBJECT
public:
    explicit MYCAMERA(QObject *parent = 0);
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

    CELL::real3    _eye;
    CELL::real3    _look;
    CELL::real3    _dir;
    CELL::real3    _up;
    CELL::real3    _right;
    CELL::real2     _viewSize;
    float           _moveSpeed;
    CELL::matrix4r  _matView;
    CELL::matrix4r  _matProj;
    CELL::matrix4r  _matWorld;


    void initMycamera(CELL::real3 eye, CELL::real3 look, CELL::real3 right, float moveSpeed);
    void setViewSize(CELL::real2 viewsize);

    const CELL::matrix4r& getProject();
    const CELL::matrix4r& getView();

    /**
    *   透视投影
    */
    void    perspective(CELL::real fovy, CELL::real aspect, CELL::real zNear, CELL::real zFar);
    CELL::Ray createRayFromScreen(int x,int y);

    void updataMycamera(float fElapsed);
    /**
    *   窗口坐标转化为世界坐标
    */
    bool    unProject( const CELL::real4& screen, CELL::real4& world );

    virtual void    scaleCameraByPos(const CELL::real3& pos,CELL::real persent);
    virtual void    rotateViewY(CELL::real angle);
    virtual void    rotateViewX(CELL::real angle);
    CELL::float3    calcIntersectPoint(CELL::Ray& ray);

    virtual void    update();

//!鼠标事件相关
    MOUSEMODE _type;

public slots:
    void reciveKeyEvent(KEYMODE type);
    void reciveMouseEvent(MOUSEMODE type, CELL::int2 pS, CELL::int2 pE);
    void reciveWheelEvent(MOUSEMODE type,int p,CELL::int2 point);
};

#endif // MYCAMERA_H
