#include "qgleswidget.h"

QGLESWIDGET::QGLESWIDGET(QWidget *parent) : QWidget(parent)
{
    leftB = false;
    rightB = false;
    typeMouse = MOUSE_NULL;
}

QGLESWIDGET::~QGLESWIDGET()
{
    destroyOpenGLES20();
}

bool QGLESWIDGET::init_QGW(std::vector<QString> fileName)
{
    if(!initOpenGLES20())
    {
        return false;
    }
    _shader.initialize();

//    glClearDepthf(1.0f);
//    glEnable(GL_DEPTH_TEST);

    QString name = fileName[0];
//    _texture = loadTextureMipmap(fileName);
    _texture = loadTexture(name.toLatin1().data());
    name = fileName[1];
    _textureAuto = loadTexture(name.toLatin1().data());
    _texture1 = createTexture(_width,_height);

    //!初始化摄像机的数据
    CELL::float3 eye   =   CELL::real3(0.0f,1000.0f,0.0f);
    CELL::float3 look  =   CELL::real3(0.0f,1.0f,0.0f);
    CELL::float3 right =   CELL::float3(1.0f,0.0f,0.0f);
    float moveSpeed    =   5.0f;

    _camera.initMycamera(eye,look,right,moveSpeed);
    _camera.update();

    connect(this,SIGNAL(sendKeyEvent(KEYMODE)),&_camera,SLOT(reciveKeyEvent(KEYMODE)));
    connect(this,SIGNAL(sendMouseEvent(MOUSEMODE,CELL::int2,CELL::int2)),&_camera,SLOT(reciveMouseEvent(MOUSEMODE,CELL::int2,CELL::int2)));
    connect(this,SIGNAL(sendWheelEvent(MOUSEMODE,int,CELL::int2)),&_camera,SLOT(reciveWheelEvent(MOUSEMODE,int,CELL::int2)));
}

bool QGLESWIDGET::initOpenGLES20()
{
    const EGLint attribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE,24,
        EGL_NONE
    };
    EGLint 	format(0);
    EGLint	numConfigs(0);
    EGLint  major;
    EGLint  minor;

    //! 1
    _display	    =	eglGetDisplay(EGL_DEFAULT_DISPLAY);

    //! 2init
    eglInitialize(_display, &major, &minor);

    //! 3
    eglChooseConfig(_display, attribs, &_config, 1, &numConfigs);

    eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format);
    //!!! 4 使opengl与qt的窗口进行绑定<this->winId()>
    _surface	    = 	eglCreateWindowSurface(_display, _config, this->winId(), NULL);

    //! 5
    EGLint attr[]   =   { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    _context 	    = 	eglCreateContext(_display, _config, 0, attr);
    //! 6
    if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE)
    {
        return false;
    }

    eglQuerySurface(_display, _surface, EGL_WIDTH,  &_width);
    eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);

    return  true;
}

void QGLESWIDGET::destroyOpenGLES20()
{
    glDeleteTextures(1,&_texture);
    if (_display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (_context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(_display, _context);
        }
        if (_surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(_display, _surface);
        }
        eglTerminate(_display);
    }
    _display    =   EGL_NO_DISPLAY;
    _context    =   EGL_NO_CONTEXT;
    _surface    =   EGL_NO_SURFACE;//asdsafsaf
}

void QGLESWIDGET::render()
{
    //! 清空缓冲区
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //! 视口，在Windows窗口指定的位置和大小上绘制OpenGL内容
    glViewport(0,0,_width,_height);

    _camera.setViewSize(CELL::real2(_width,_height));
    _camera.perspective(45,(float)_width/(float)_height,1,1000);

    _shader.begin();
    {
        //!绘制一个地面
        displayGround();
        displayRect();
    }
    _shader.end();

}

unsigned int QGLESWIDGET::loadTexture(const char *fileName)
{
    unsigned int textureId   =   0;
    //1 获取图片格式
    FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);

    //2 加载图片
    FIBITMAP    *dib = FreeImage_Load(fifmt, fileName,0); 

    int fmt = GL_RGB;
    //4 获取数据指针
    BYTE    *pixels =   (BYTE*)FreeImage_GetBits(dib);

    int     width   =   FreeImage_GetWidth(dib);
    int     height  =   FreeImage_GetHeight(dib);

    /**
     *注意：1、当启用alpha测试时，那么文理格式就是（GL_RGBA），
     *       需要把图像转化成32位图，同时改变（glTexImage2D）中相对应的参数
     *     2、可根据文理的文件格式进行判断，然后确定转化成24位图还是转换成32位图，也就是是否存在alpha值
     */

    if(fifmt == FIF_PNG)
    {
        dib =   FreeImage_ConvertTo32Bits(dib);
        fmt =   GL_RGBA;
        for (size_t i = 0 ;i < width * height * 4 ; i+=4 )
        {
            BYTE temp       =   pixels[i];
            pixels[i]       =   pixels[i + 2];
            pixels[i + 2]   =   temp;
        }
    }
    else
    {
        //3 转化为rgb 24色
        dib     =   FreeImage_ConvertTo24Bits(dib);
//        fmt = GL_RGB;
        for (size_t i = 0 ;i < width * height * 3 ; i+=3 )
        {
            BYTE temp       =   pixels[i];
            pixels[i]       =   pixels[i + 2];
            pixels[i + 2]   =   temp;
        }
    }


    /**
        *   产生一个纹理Id,可以认为是纹理句柄，后面的操作将书用这个纹理id
        */
    glGenTextures( 1, &textureId );

    /**
        *   使用这个纹理id,或者叫绑定(关联)
        */
    glBindTexture( GL_TEXTURE_2D, textureId );
    /**
        *   指定纹理的放大,缩小滤波，使用线性方式，即当图片放大的时候插值方式
        */
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    /**
        *   将图片的rgb数据上传给opengl.
        */
    glTexImage2D(
                GL_TEXTURE_2D,      //! 指定是二维图片
                0,                  //! 指定为第一级别，纹理可以做mipmap,即lod,离近的就采用级别大的，远则使用较小的纹理
                fmt,                //! 纹理的使用的存储格式
                width,              //! 宽度，老一点的显卡，不支持不规则的纹理，即宽度和高度不是2^n。
                height,             //! 宽度，老一点的显卡，不支持不规则的纹理，即宽度和高度不是2^n。
                0,                  //! 是否存在边
                fmt,                //! 数据的格式，bmp中，windows,操作系统中存储的数据是bgr格式
                GL_UNSIGNED_BYTE,   //! 数据是8bit数据
                pixels
                );
    /**
        *   释放内存
        */
    FreeImage_Unload(dib);

    return  textureId;
}

unsigned int QGLESWIDGET::loadTextureMipmap(std::vector<QString> fName)
{
    //!函数实现功能：把多个文理尺寸放在同一个文理中，
    //!实现各种显示出来的效果（有文理分明的过度和渐变过度，主要是调整<glTexParameteri>函数中的最后一个参数来实现）????????????
    unsigned int textureId = 0;
    glGenTextures(1,&textureId);
    glBindTexture( GL_TEXTURE_2D, textureId );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    int fileCount = fName.size();
    printf("file number:%d\n",fileCount);
    fflush(NULL);
    for(int j=0;j<fileCount;++j)
    {
        QString fileName = fName[j];
        FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName.toLatin1().data(), 0);

        //2 加载图片
        FIBITMAP    *dib = FreeImage_Load(fifmt,fileName.toLatin1().data(),0);

        //3 转化为rgb 24色
        dib     =   FreeImage_ConvertTo24Bits(dib);

        //4 获取数据指针
        BYTE    *pixels =   (BYTE*)FreeImage_GetBits(dib);

        int     width   =   FreeImage_GetWidth(dib);
        int     height  =   FreeImage_GetHeight(dib);

        for (size_t i = 0 ;i < width * height * 3 ; i+=3 )
        {
            BYTE temp       =   pixels[i];
            pixels[i]       =   pixels[i + 2];
            pixels[i + 2]   =   temp;
        }
        glTexImage2D(
                    GL_TEXTURE_2D,      //! 指定是二维图片
                    j,                  //! 指定为第一级别，纹理可以做mipmap,即lod,离近的就采用级别大的，远则使用较小的纹理
                    GL_RGB,             //! 纹理的使用的存储格式
                    width,              //! 宽度，老一点的显卡，不支持不规则的纹理，即宽度和高度不是2^n。
                    height,             //! 宽度，老一点的显卡，不支持不规则的纹理，即宽度和高度不是2^n。
                    0,                  //! 是否存在边
                    GL_RGB,             //! 数据的格式，bmp中，windows,操作系统中存储的数据是bgr格式
                    GL_UNSIGNED_BYTE,   //! 数据是8bit数据
                    pixels
                    );
        /**
            *   释放内存
            */
        FreeImage_Unload(dib);
    }

    return textureId;
}

unsigned int QGLESWIDGET::createTexture(int width, int height)
{
    //!函数功能：产生一个新的空白文理，这个文理产生时是黑色的
    unsigned int textureId;
    glGenTextures(1,&textureId);//!生成文理
    glBindTexture(GL_TEXTURE_2D,textureId);//绑定文理
    /**
     *以下函数（）说明：这是一个设置文理参数的函数，同时也是起到对文理的包装
     * 对文理的放大、缩小滤波起到多层文理的过度作用
     * 主要说明最后一个参数：
     * 当第二个参数是<GL_TEXTURE_MAG_FILTER>时，
     * 参数3：GL_LINEAR
     * 参数3：GL_NEAREST
     * 当第二个参数是<GL_TEXTURE_MIN_FILTER>时，
     * 参数3：GL_LINEAR
     * 参数3：GL_NEAREST
     * 参数3：GL_LINEAR_MIPMAP_LINEAR 针对多层文理可用
     * 参数3：GL_NEAREST_MIPMAP_LINEAR 针对多层文理可用
     * 参数3：GL_LINEAR_MIPMAP_NEAREST 针对多层文理可用
     * 参数3：GL_NEAREST_MIPMAP_NEAREST 针对多层文理可用
     */
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);//设置文理的相关参数1、文理的放大滤波
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);//设置文理的相关参数2、文理的缩小滤波
    /**
     *针对文理的包装:
     * 参数2是<GL_TEXTURE_WRAP_S>时，表示水平文理，
     *       <GL_TEXTURE_WRAP_T>时，表示垂直文理
     * 参数3：GL_REPEAT        表示文理根据所给文理重复
     * 参数3：GL_CLAMP_TO_EDGE 表示文理根据所给文理的最后一个像素重复
     * 参数3：GL_REPEAT        表示文理根据所给文理镜像重复
     */
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);//设置文理的相关参数3、水平文理的重复格式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//设置文理的相关参数4、垂直文理的重复格式
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,0);
    return textureId;
}


void QGLESWIDGET::drawImage()
{
    render();
    eglSwapBuffers(_display,_surface);
}


void QGLESWIDGET::displayGround()
{
    float   gSizeX = 500;
    float   gSizeY = 500;
    float   gPos = 0;
    float   rept = 1;

    Vertex grounds[] =
    {
        { -gSizeX, gPos,-gSizeY, 0,-1,0, 0.0f, 0.0f},
        {  gSizeX, gPos,-gSizeY, 0,-1,0, rept, 0.0f},
        {  gSizeX, gPos, gSizeY, 0,-1,0, rept, rept},

        { -gSizeX, gPos,-gSizeY, 0,-1,0, 0.0f, 0.0f},
        {  gSizeX, gPos, gSizeY, 0,-1,0, rept, rept},
        { -gSizeX, gPos, gSizeY, 0,-1,0, 0.0f, rept},
    };
//    _camera.setViewSize(CELL::real2(_width,_height));
//    _camera.perspective(45,(float)_width/(float)_height,1,100000);
    CELL::matrix4   mvp =   _camera.getProject() * _camera.getView();
//    _shader.begin();
    glUniformMatrix4fv(_shader._MVP,1,false,mvp.data());
    glUniform3f(_shader._lightDir,_camera._dir.x,_camera._dir.y,_camera._dir.z);

    glUniform3f(_shader._color,0.0f,1.0f,0.0f);

    glVertexAttribPointer(_shader._position, 3, GL_FLOAT, false, sizeof(Vertex), grounds);
    glVertexAttribPointer(_shader._normal, 3, GL_FLOAT, false, sizeof(Vertex), &grounds[0].nx);
    glDrawArrays(GL_TRIANGLES,0,6);


}

void QGLESWIDGET::displayRect()
{
    static  float   angle   =   0;
    CELL::float3 halfSize(20,20,20);

    CELL::float3 A(-1,1,1);
    CELL::float3 B(-1,-1,1);
    CELL::float3 C(1,-1,1);
    CELL::float3 D(1,1,1);
    CELL::float3 A1(-1,1,-1);
    CELL::float3 B1(-1,-1,-1);
    CELL::float3 C1(1,-1,-1);
    CELL::float3 D1(1,1,-1);

    A*=halfSize;
    B*=halfSize;
    C*=halfSize;
    D*=halfSize;
    A1*=halfSize;
    B1*=halfSize;
    C1*=halfSize;
    D1*=halfSize;

    V3N3  verts[] =
    {//!绘制矩形，设置矩形的顶点坐标
     //!1个矩形由6个面组成，1个面由2个三角形组成，1个三角形由3个点组成<6*2*3 = 36>
     //!36个点坐标规律：在一个三角形中一个轴坐标符号全部相同时，轴符号取反是法线的方向，法线值为1。
        { A.x,   A.y, A.z,  0.0f, 0.0f,  -1.0f },
        { B.x,   B.y, B.z,  0.0f, 0.0f,  -1.0f },
        { C.x,   C.y, C.z,  0.0f, 0.0f,  -1.0f },

        { A.x,   A.y, A.z,  0.0f,  0.0f, -1.0f },
        { C.x,   C.y, C.z,  0.0f,  0.0f, -1.0f },
        { D.x,   D.y, D.z,  0.0f,  0.0f, -1.0f },

        { C1.x,   C1.y, C1.z, 0.0f,  0.0f, 1.0f },
        { B1.x,   B1.y, B1.z, 0.0f,  0.0f, 1.0f },
        { A1.x,   A1.y, A1.z, 0.0f,  0.0f, 1.0f },

        { C1.x,   C1.y, C1.z,  0.0f, 0.0f, 1.0f },
        { A1.x,   A1.y, A1.z,  0.0f, 0.0f, 1.0f },
        { D1.x,   D1.y, D1.z,  0.0f, 0.0f, 1.0f },

        { D.x,    D.y,  D.z,    -1.0f,  0.0f,  0.0f },
        { C.x,    C.y,  C.z,    -1.0f,  0.0f,  0.0f },
        { C1.x,   C1.y, C1.z,   -1.0f,  0.0f,  0.0f },

        { D.x,    D.y,  D.z,    -1.0f,  0.0f,  0.0f },
        { C1.x,   C1.y, C1.z,   -1.0f,  0.0f,  0.0f },
        { D1.x,   D1.y, D1.z,   -1.0f,  0.0f,  0.0f },

        { B1.x,  B1.y,B1.z,      1.0f, 0.0f,  0.0f },
        { B.x,   B.y, B.z,      1.0f, 0.0f,  0.0f },
        { A .x,  A .y, A .z,     1.0f, 0.0f,  0.0f },

        { B1.x,   B1.y, B1.z,  1.0f,  0.0f, 0.0f },
        { A .x,   A .y, A .z,  1.0f,  0.0f, 0.0f },
        { A1.x,   A1.y, A1.z,  1.0f,  0.0f, 0.0f },

        { A1.x,   A1.y, A1.z, 0.0f,  -1.0f,  0.0f },//y,z
        { A .x,   A .y, A .z, 0.0f,  -1.0f,  0.0f },//+x
        { D .x,   D .y, D .z, 0.0f,  -1.0f,  0.0f },

        { A1.x,   A1.y, A1.z,  0.0f,  -1.0f, +1.0f },
        { D .x,   D .y, D .z,  0.0f,  -1.0f, +1.0f },
        { D1.x,   D1.y, D1.z,  0.0f,  -1.0f, +1.0f },

        { B1.x,   B1.y, B1.z, 0.0f,  1.0f,  0.0f },//y,z
        { C .x,   C .y, C .z, 0.0f,  1.0f,  0.0f },//-x
        { B.x,    B.y,  B.z,  0.0f,  1.0f,  0.0f },

        { B1.x,   B1.y, B1.z,  0.0f, 1.0f,  0.0f },//x,z
        { C1.x,   C1.y, C1.z,  0.0f, 1.0f,  0.0f },//+y
        { C .x,   C .y, C .z,  0.0f, 1.0f,  0.0f },
    };

    CELL::matrix4   mvp1 =   _camera.getProject() * _camera.getView();
    angle   +=  0.1f;
    CELL::matrix4   matRot;
    CELL::matrix4   matTrans;
    matTrans.translate(0,10,0);
    matRot.rotateYXZ(angle, angle, angle);
    CELL::matrix4    mvp = mvp1 * (matTrans *matRot);
    glUniformMatrix4fv(_shader._MVP, 1, GL_FALSE, mvp.data());

    glUniform3f(_shader._color,1.0f,1.0f,1.0f);

    glVertexAttribPointer(_shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(V3N3), verts);
    glVertexAttribPointer(_shader._normal, 3, GL_FLOAT, GL_FALSE, sizeof(V3N3), &verts[0].nx);
    glDrawArrays(GL_TRIANGLES, 0, 36);

//#if 0
    LINNES line[] =
    {
        A.x, A.y, A.z,
        B.x, B.y, B.z,
        C.x, C.y, C.z,
        D.x, D.y, D.z,
    };
    glUniformMatrix4fv(_shader._MVP, 1, GL_FALSE, mvp.data());

    glUniform3f(_shader._color,1.0f,0.0f,0.0f);

    glVertexAttribPointer(_shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(LINNES), line);
    glVertexAttribPointer(_shader._normal, 3, GL_FLOAT, GL_FALSE, sizeof(V3N3), &verts[0].nx);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    LINNES line1[] =
    {
        A1.x,A1.y,A1.z,
        B1.x,B1.y,B1.z,
        C1.x,C1.y,C1.z,
        D1.x,D1.y,D1.z,
    };
    glUniformMatrix4fv(_shader._MVP, 1, GL_FALSE, mvp.data());

    glUniform3f(_shader._color,0.0f,1.0f,0.0f);

    glVertexAttribPointer(_shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(LINNES), line1);
    glVertexAttribPointer(_shader._normal, 3, GL_FLOAT, GL_FALSE, sizeof(V3N3), &verts[0].nx);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    LINNES line2[] =
    {
        A1.x,A1.y,A1.z,
        A.x, A.y, A.z,
        B.x, B.y, B.z,
        B1.x,B1.y,B1.z,
    };
    glUniformMatrix4fv(_shader._MVP, 1, GL_FALSE, mvp.data());

    glUniform3f(_shader._color,0.0f,0.0f,1.0f);

    glVertexAttribPointer(_shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(LINNES), line2);
    glVertexAttribPointer(_shader._normal, 3, GL_FLOAT, GL_FALSE, sizeof(V3N3), &verts[0].nx);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    LINNES line3[] =
    {
        C1.x,C1.y,C1.z,
        D1.x,D1.y,D1.z,
        D.x, D.y, D.z,
        C.x, C.y, C.z,
    };
    glUniformMatrix4fv(_shader._MVP, 1, GL_FALSE, mvp.data());

    glUniform3f(_shader._color,1.0f,1.0f,0.0f);

    glVertexAttribPointer(_shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(LINNES), line3);
    glVertexAttribPointer(_shader._normal, 3, GL_FLOAT, GL_FALSE, sizeof(V3N3), &verts[0].nx);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    LINNES line4[] =
    {
        A1.x,A1.y,A1.z,
        A.x, A.y, A.z,
        D.x, D.y, D.z,
        D1.x,D1.y,D1.z,
    };
    glUniformMatrix4fv(_shader._MVP, 1, GL_FALSE, mvp.data());

    glUniform3f(_shader._color,0.0f,1.0f,1.0f);

    glVertexAttribPointer(_shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(LINNES), line4);
    glVertexAttribPointer(_shader._normal, 3, GL_FLOAT, GL_FALSE, sizeof(V3N3), &verts[0].nx);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    LINNES line5[] =
    {
        B.x, B.y, B.z,
        C.x, C.y, C.z,
        C1.x,C1.y,C1.z,
        B1.x,B1.y,B1.z,
    };
    glUniformMatrix4fv(_shader._MVP, 1, GL_FALSE, mvp.data());

    glUniform3f(_shader._color,1.0f,0.0f,1.0f);

    glVertexAttribPointer(_shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(LINNES), line5);
    glVertexAttribPointer(_shader._normal, 3, GL_FLOAT, GL_FALSE, sizeof(V3N3), &verts[0].nx);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//#endif
}


bool QGLESWIDGET::eventFilter(QObject *target, QEvent *event)
{
    if( target == parent )
     {
         if( event->type() == QEvent::KeyPress )
         {
             QKeyEvent *ke = (QKeyEvent *) event;
             keyPressEvent(ke);
          }
         if( event->type() == QEvent::MouseTrackingChange )
         {
             QMouseEvent *mouse = (QMouseEvent *) event;
             mouseMoveEvent(mouse);
             mousePressEvent(mouse);
             mouseReleaseEvent(mouse);
             QWheelEvent *wheel = (QWheelEvent *) event;
             wheelEvent(wheel);
         }
     }
    return true;
}

void QGLESWIDGET::keyPressEvent(QKeyEvent *e)
{
//    printf("keyPressEvent\n");
//    fflush(NULL);
    KEYMODE type = KEY_NULL;
    if(e->key()==Qt::Key_A)
    {

        type = KEY_A;
    }
    else if(e->key()==Qt::Key_S)
    {
        type = KEY_S;
    }
    else if(e->key()==Qt::Key_D)
    {
        type = KEY_D;
    }
    else if(e->key()==Qt::Key_W)
    {
        type = KEY_W;
    }
    emit sendKeyEvent(type);
}

void QGLESWIDGET::mouseMoveEvent(QMouseEvent *event)
{
    CELL::int2 point;
    if(leftB)
    {
        point = CELL::int2(event->x(),event->y());
    }
    if(rightB)
    {
        point = CELL::int2(event->x(),event->y());
    }
    emit sendMouseEvent(typeMouse,pos,point);
    pos = point;
}

void QGLESWIDGET::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !leftB)
    {
        leftB       = true;
        typeMouse   = MOUSE_LEFTDOWN;
        pos         = CELL::int2(event->x(),event->y());
    }
    if(event->button() == Qt::RightButton && !rightB)
    {
        rightB      = true;
        typeMouse   = MOUSE_RIGHTDOWN;
        pos         = CELL::int2(event->x(),event->y());
    }
}

void QGLESWIDGET::mouseReleaseEvent(QMouseEvent *event)
{
    MOUSEMODE type;
    CELL::int2 point;
    if(event->button() == Qt::LeftButton && leftB)
    {
        leftB   = false;
        type    = MOUSE_LEFTUP;
        point   = CELL::int2(event->x(),event->y());
    }
    if(event->button() == Qt::RightButton && rightB)
    {
        rightB  = false;
        type    = MOUSE_RIGHTUP;
        point   = CELL::int2(event->x(),event->y());
    }
    emit sendMouseEvent(type,pos,point);
    pos = point;
}

void QGLESWIDGET::wheelEvent(QWheelEvent *event)
{
    typeMouse = MOUSE_WHEEL;
    int p = event->delta();
    CELL::int2 pp = CELL::int2(event->x(),event->y());
    emit sendWheelEvent(typeMouse,p,pp);
}


