#include "cube.h"

Cube::Cube(QWidget *parent)
    : QOpenGLWidget(parent), m_texture(0), m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    m_x = 0.0f;
    m_y = 0.0f;
    m_z = -5.0f;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(moveCube()));
    timer->start(33);
}

Cube::~Cube()
{
}

void Cube::initializeGL()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();
    initCube(1.0f);
}

void Cube::resizeGL(int w, int h)
{
    float aspect = w / (float)h;

    _mat.setToIdentity();
    _mat.perspective(45,aspect,0.1f,10.0f);
}

void Cube::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(m_x,m_y,m_z);
    modelViewMatrix.rotate(m_rotation);

    m_texture->bind(0);

    m_program.bind();
    m_program.setUniformValue("qt_ModelViewProjectionMatrix",_mat * modelViewMatrix);
    m_program.setUniformValue("qt_Texture0",0);

    m_arrayBuffer.bind();

    int offset = 0;

    int vertLoc = m_program.attributeLocation("qt_Vertex");
    m_program.enableAttributeArray(vertLoc);
    m_program.setAttributeBuffer(vertLoc,GL_FLOAT,offset,3,sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texLoc = m_program.attributeLocation("qt_MultiTexCoord0");
    m_program.enableAttributeArray(texLoc);
    m_program.setAttributeBuffer(texLoc,GL_FLOAT,offset,2,sizeof(VertexData));

    m_indexBuffer.bind();

    glDrawElements(GL_TRIANGLES,m_indexBuffer.size(),GL_UNSIGNED_INT,0);
}

void Cube::initShaders()
{
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,":/vshader.vsh")){
        close();
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment,":/fshader.fsh")){
        close();
    }
    if (!m_program.link()){
        close();
    }
}

void Cube::initCube(float width)
{
    float width_div_2 = width/2;
    QVector<VertexData> vertexes;
    vertexes.append(VertexData(QVector3D(-width_div_2,width_div_2,width_div_2),QVector2D(0.0,1.0),QVector3D(0.0,0.0,1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,-width_div_2,width_div_2),QVector2D(0.0,0.0),QVector3D(0.0,0.0,1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,width_div_2,width_div_2),QVector2D(1.0,1.0),QVector3D(0.0,0.0,1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-width_div_2,width_div_2),QVector2D(1.0,0.0),QVector3D(0.0,0.0,1.0)));

    vertexes.append(VertexData(QVector3D(width_div_2,width_div_2,width_div_2),QVector2D(0.0,1.0),QVector3D(1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-width_div_2,width_div_2),QVector2D(0.0,0.0),QVector3D(1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,width_div_2,-width_div_2),QVector2D(1.0,1.0),QVector3D(1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-width_div_2,-width_div_2),QVector2D(1.0,0.0),QVector3D(1.0,0.0,0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2,width_div_2,width_div_2),QVector2D(0.0,1.0),QVector3D(0.0,1.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,width_div_2,-width_div_2),QVector2D(0.0,0.0),QVector3D(0.0,1.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,width_div_2,width_div_2),QVector2D(1.0,1.0),QVector3D(0.0,1.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,width_div_2,-width_div_2),QVector2D(1.0,0.0),QVector3D(0.0,1.0,0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2,width_div_2,-width_div_2),QVector2D(0.0,1.0),QVector3D(0.0,0.0,-1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-width_div_2,-width_div_2),QVector2D(0.0,0.0),QVector3D(0.0,0.0,-1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,width_div_2,-width_div_2),QVector2D(1.0,1.0),QVector3D(0.0,0.0,-1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,-width_div_2,-width_div_2),QVector2D(1.0,0.0),QVector3D(0.0,0.0,-1.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2,width_div_2,width_div_2),QVector2D(0.0,1.0),QVector3D(-1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,width_div_2,-width_div_2),QVector2D(0.0,0.0),QVector3D(-1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,-width_div_2,width_div_2),QVector2D(1.0,1.0),QVector3D(-1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,-width_div_2,-width_div_2),QVector2D(1.0,0.0),QVector3D(-1.0,0.0,0.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2,-width_div_2,width_div_2),QVector2D(0.0,1.0),QVector3D(0.0,-1.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,-width_div_2,-width_div_2),QVector2D(0.0,0.0),QVector3D(0.0,-1.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-width_div_2,width_div_2),QVector2D(1.0,1.0),QVector3D(0.0,-1.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-width_div_2,-width_div_2),QVector2D(1.0,0.0),QVector3D(0.0,-1.0,0.0)));

    QVector<GLuint> indexes;
    for (int i = 0; i < 24; i += 4){
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    m_arrayBuffer.create();
    m_arrayBuffer.bind();
    m_arrayBuffer.allocate(vertexes.constData(),vertexes.size() * sizeof(VertexData));
    m_arrayBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(),indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(QImage(":/cube.jpg").mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);

}

void Cube::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W){
        keyboard[Qt::Key_W] = up;
    } else if (event->key() == Qt::Key_A){
        keyboard[Qt::Key_A] = left;
    } else if (event->key() == Qt::Key_S){
        keyboard[Qt::Key_S] = down;
    } else if (event->key() == Qt::Key_D){
        keyboard[Qt::Key_D] = right;
    }
    update();
}

void Cube::keyReleaseEvent(QKeyEvent *event)
{
    keyboard.remove(event->key());
}

void Cube::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0){
        m_z += 0.25;
    } else if (event->delta() < 0){
        m_z -= 0.25;
    }
    update();
}

void Cube::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
        m_mousePosition = QVector2D(event->localPos());
    event->accept();
}

void Cube::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton) return;

    QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->localPos());
    float angle = diff.length() / 2.0f;

    QVector3D axis = QVector3D(diff.y(),diff.x(),0.0f);


    m_rotation = QQuaternion::fromAxisAndAngle(axis,angle) * m_rotation;
    update();
}

void Cube::up(Cube* c)
{
    c->m_y += 0.05f;
}

void Cube::down(Cube* c)
{
    c->m_y -= 0.05f;
}

void Cube::left(Cube* c)
{
    c->m_x -= 0.05f;
}

void Cube::right(Cube* c)
{
    c->m_x += 0.05f;
}

void Cube::moveCube()
{
    for (const auto& elem : keyboard){
        elem(this);
    }
    update();
}

