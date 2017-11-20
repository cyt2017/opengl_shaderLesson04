#include "mycamera.h"

MYCAMERA::MYCAMERA(QObject *parent) : QObject(parent)
{

}

void MYCAMERA::initMycamera(CELL::real3 eye, CELL::real3 look, /*CELL::real3 up,*/ CELL::real3 right, float moveSpeed)
{
    _matView    = CELL::matrix4r(1);
    _matProj    = CELL::matrix4r(1);
    _matWorld   = CELL::matrix4r(1);

    _eye        = eye;
    _look       = look;
    _right      = right;
    _dir        = CELL::normalize(_look - _eye);
    _up         = CELL::normalize(CELL::cross(_right,_dir));


    _moveSpeed  = moveSpeed;
    _viewSize   = CELL::real2(600,600);
}

void MYCAMERA::setViewSize(CELL::real2 viewsize)
{
    _viewSize = viewsize;
}

const CELL::matrix4r& MYCAMERA::getProject()
{
    return _matProj;
}

const CELL::matrix4r& MYCAMERA::getView()
{
    return _matView;
}

void MYCAMERA::perspective(CELL::real fovy, CELL::real aspect, CELL::real zNear, CELL::real zFar)
{
    _matProj    =   CELL::perspective<CELL::real>(fovy,aspect,zNear,zFar);
}

CELL::Ray MYCAMERA::createRayFromScreen(int x, int y)
{
    CELL::real4  minWorld;
    CELL::real4  maxWorld;

    CELL::real4  screen(CELL::real(x),CELL::real(y),0,1);
    CELL::real4  screen1(CELL::real(x),CELL::real(y),1,1);

    unProject(screen,minWorld);
    unProject(screen1,maxWorld);
    CELL::Ray     ray;
    ray.setOrigin(CELL::real3(minWorld.x,minWorld.y,minWorld.z));

    CELL::real3  dir(maxWorld.x - minWorld.x,maxWorld.y - minWorld.y, maxWorld.z - minWorld.z);
    ray.setDirection(CELL::normalize(dir));
    return  ray;
}

void MYCAMERA::updataMycamera(float fElapsed)
{
//    CELL::float3    dir     =   _look - _eye;
//    dir =   CELL::normalize(dir);
//    switch (_type) {
//    case KEY_A:
//    {
////        _eye    -=  (_right* _moveSpeed) * fElapsed;
////        _look   -=  (_right* _moveSpeed) * fElapsed;
//    }
//        break;
//    case KEY_S:
//    {
//        _eye    +=  (dir*-_moveSpeed) * fElapsed;
//        _look   +=  (dir*-_moveSpeed) * fElapsed;
//    }
//        break;
//    case KEY_D:
//    {
////        _eye    +=  (_right*_moveSpeed) * fElapsed;
////        _look   +=  (_right*_moveSpeed) * fElapsed;
//       }
//        break;
//    case KEY_W:
//       {
//            _eye    -=  dir*-_moveSpeed * fElapsed;
//            _look   -=  dir*-_moveSpeed * fElapsed;
//       }
//        break;
//    default:
//        break;
//    }
//    if(_type!=KEY_NULL)
//    {
//        printf("eye:%.3f,%.3f,%.3f\nlook:%.3f,%.3f,%.3f\n",_eye.x,_eye.y,_eye.z,_look.x,_look.y,_look.z);
//        fflush(NULL);
//    }
//    _type=KEY_NULL;

}

bool MYCAMERA::unProject(const CELL::real4 &screen, CELL::real4 &world)
{//!从屏幕坐标转换到世界坐标

    CELL::real4 v;
    v.x =   screen.x;
    v.y =   screen.y;
    v.z =   screen.z;
    v.w =   1.0;

    // map from viewport to 0 - 1
    v.x =   (v.x) /_viewSize.x;
    v.y =   (_viewSize.y - v.y) /_viewSize.y;


    // map to range -1 to 1
    v.x =   v.x * 2.0f - 1.0f;
    v.y =   v.y * 2.0f - 1.0f;
    v.z =   v.z * 2.0f - 1.0f;

    CELL::matrix4r  inverse = (_matProj * _matView * _matWorld).inverse();

    v   =   v * inverse;
    if (v.w == 0.0f)
    {
        return false;
    }
    world   =   v / v.w;
    return true;
}


void MYCAMERA::scaleCameraByPos(const CELL::real3 &pos, CELL::real persent)
{

    CELL::real3   dir     =   CELL::normalize(pos - _eye);

    CELL::real    dis     =   CELL::length(pos - _eye) * persent;

    CELL::real    disCam  =   CELL::length(_look - _eye) * persent;

    CELL::real3   dirCam  =   CELL::normalize(_look - _eye);

    _eye    =   pos - dir * dis;
    _look =   _eye + dirCam * disCam;

    update();
}

void MYCAMERA::rotateViewY(CELL::real angle)
{
    CELL::real        len(0);
    CELL::matrix4r    mat(1);
    mat.rotate(angle, CELL::real3(0, 1, 0));
    _dir        =   _dir * mat;
    _up         =   _up * mat;
    _right      =   CELL::normalize(CELL::cross(_dir, _up));
    len         =   CELL::length(_eye - _look);
    _eye        =   _look - _dir * len;
    _matView    =   CELL::lookAt<CELL::real>(_eye, _look, _up);
}

void MYCAMERA::rotateViewX(CELL::real angle)
{
    CELL::real        len(0);
    CELL::matrix4r    mat(1);
    mat.rotate(angle,_right);
    _dir        =   _dir * mat;
    _up         =   _up * mat;
    _right      =   CELL::normalize(CELL::cross(_dir,_up));
    len         =   CELL::length(_eye - _look);
    _eye        =   _look - _dir * len;
    _matView    =   CELL::lookAt(_eye,_look,_up);
}

CELL::float3 MYCAMERA::calcIntersectPoint(CELL::Ray &ray)
{
    CELL::float3   pos = ray.getOrigin();
    float    tm = abs((pos.y) / ray.getDirection().y);
    CELL::float3   target = ray.getPoint(tm);
    return   CELL::float3(target.x, 0, target.z);
}

void MYCAMERA::update()
{
    _matView    =   CELL::lookAt(_eye,_look,_up);
}

void MYCAMERA::reciveKeyEvent(KEYMODE type)
{
//    _type = type;
}

void MYCAMERA::reciveMouseEvent(MYCAMERA::MOUSEMODE type, CELL::int2 pS, CELL::int2 pE)
{
    CELL::int2 pointStart = pS;
    if(type == MOUSE_LEFTDOWN)
    { /**
        *   首先计算出来一个像素和当前场景的比例
        */
        CELL::Ray   ray0      = createRayFromScreen(pE.x,pE.y);
        CELL::Ray   ray1      = createRayFromScreen(pointStart.x,pointStart.y);

        CELL::real3  pos0     =   calcIntersectPoint(ray0);
        CELL::real3  pos1     =   calcIntersectPoint(ray1);

        CELL::real3  offset = pos1 - pos0;

        CELL::real3  newEye   =  _eye + offset;
        CELL::real3  newTgt   =  _look + offset;

        _eye = newEye;
        _look = newTgt;
    }
    if(type == MOUSE_RIGHTDOWN)
    {

        CELL::int2    offset = pE - pS;
        rotateViewY(offset.x * 0.5f);
        rotateViewX(offset.y * 0.5f);
    }
    update();
}

void MYCAMERA::reciveWheelEvent(MYCAMERA::MOUSEMODE type, int p, CELL::int2 point)
{
    CELL::real    persent =   p > 0 ? 1.1f : 0.9f;
    CELL::Ray     ray     =   createRayFromScreen(point.x, point.y);
    CELL::float3  pos     =   ray.getOrigin();
    float   tm      =   abs((pos.y - 0) / ray.getDirection().y);
    CELL::float3  center  =   ray.getPoint(tm);
    center.y = 0;

    scaleCameraByPos(center, persent);
}

