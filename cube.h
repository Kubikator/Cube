    #ifndef CUBE_H
#define CUBE_H

#include <QWidget>
#include<QOpenGLWidget>
#include<QMatrix4x4>
#include<QOpenGLShaderProgram>
#include<QOpenGLTexture>
#include<QOpenGLBuffer>
#include<QMouseEvent>
#include<QKeyEvent>
#include<QWheelEvent>
#include<QTimer>

struct VertexData{
    VertexData()
    {
    }
    VertexData(QVector3D p,QVector2D t,QVector3D n) :
        position(p),texCoord(t),normal(n)
    {
    }
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

class Cube : public QOpenGLWidget
{
    Q_OBJECT

public:
    Cube(QWidget *parent = nullptr);
    ~Cube();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void initShaders();
    void initCube(float width);


    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    static void up(Cube* c);
    static void down(Cube* c);
    static void left(Cube* c);
    static void right(Cube* c);

private slots:
    void moveCube();

private:
    QMap<int,void(*)(Cube* c)> keyboard;
    QMatrix4x4 _mat;
    QOpenGLShaderProgram m_program;
    QOpenGLTexture* m_texture;
    QOpenGLBuffer m_arrayBuffer;
    QOpenGLBuffer m_indexBuffer;
    QVector2D m_mousePosition;
    QQuaternion m_rotation;
    float m_x;
    float m_y;
    float m_z;
    QTimer* timer;
};
#endif // CUBE_H
