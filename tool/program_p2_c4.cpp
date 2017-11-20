#include "program_p2_c4.h"

PROGRAM_P2_C4::PROGRAM_P2_C4()
{
    _position   =   -1;
    _normal     =   -1;
    _color      =   -1;
    _MVP        =   -1;
    _lightDir   =   -1;
}

bool PROGRAM_P2_C4::initialize()
{
    //!使用（varying），是表示需要在（vs）和（ps）之间进行传递的变量，把需要传递的变量在（vs）中赋值，并在（ps）中重复使用（varying）语句即可
    const char* vs  =
    {
        "uniform    mat4    _MVP;"
        "uniform    vec3    _lightDir;"
        "uniform    vec3    _color;"
        "attribute  vec3    _position;"
        "attribute  vec3    _normal;"

        "varying    vec4    _outColor;"

        "void main()"
        "{"
        "   float NDotL =   max(dot(_normal, _lightDir), 0.1);"
        "   _outColor    =   vec4(_color*NDotL,1);"
        "   vec4    pos =   vec4(_position,1);"
        "   gl_Position =   _MVP * pos;"
        "}"
    };
    const char* ps  =
    {
        "precision  lowp    float; "
        "varying    vec4    _outColor;"
        "void main()"
        "{"
        "   gl_FragColor=   _outColor;"
        "}"
    };
    //!上面的if语句，表示如果颜色中的alpha值<0.2,那么就不绘制（discard）出来。
    //!在上面的（"gl_FragColor=tcolor0 + tcolor1;"）中，用（+）图像变亮；用（*）图像变暗。
    bool    res =   createProgram(vs,ps);
    if(res)
    {
        //!~需要注意变量的类型
        _position   =   glGetAttribLocation(_programId,"_position");
        _normal     =   glGetAttribLocation(_programId,"_normal");
        _color      =   glGetUniformLocation(_programId,"_color");
        _MVP        =   glGetUniformLocation(_programId,"_MVP");
        _lightDir   =   glGetUniformLocation(_programId,"_lightDir");
    }
    return  res;
}

void PROGRAM_P2_C4::begin()
{
    glUseProgram(_programId);
    //!在显卡里面使用的局部变量，在使用时是需要进行使能和关闭的
    glEnableVertexAttribArray(_position);       
    glEnableVertexAttribArray(_normal);
}

void PROGRAM_P2_C4::end()
{
    glDisableVertexAttribArray(_position);
    glDisableVertexAttribArray(_normal);
    glUseProgram(0);
}
