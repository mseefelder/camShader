#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <rendertexture.hpp>
#include <utils/qtplainwidget.hpp>
#include <opencv/cv.hpp>
#include <highgui.h>
#include "cameraexception.hpp"
#include "freeze.hpp"

using namespace std;

class GLWidget : public Tucano::QtPlainWidget
{
    Q_OBJECT

protected:
    /// VARIABLES
    
    /// Is initialized
    bool initd;
    // Rendering quad
    Tucano::Mesh *quad;

    /// Path where shaders are stored
    string shaders_dir;

    ///Max camera search index
    int maxCamIndex;

    /// FUNCTIONS

public:

    /// VARIABLES

    /// FUNCTIONS
    explicit GLWidget(QWidget *parent);
    ~GLWidget();
    
    /**
     * @brief Initializes the shader effect
     */
    void initialize();

    /**
     * Repaints screen buffer.
     */
    virtual void paintGL();

    void resizeGL(int w, int h)
    {
        resize(w, h);
        glViewport(0, 0, this->width(), this->height());
        viewport = Eigen::Vector2i(this->width(), this->height());

        updateGL();
    }

signals:

public slots:

private:
    //VARIABLES
    
    ///camera in use
    cv::VideoCapture* camera;
    
    ///frame where we read the camera frames to
    cv::Mat frame;
    
    ///if we want to get another camera, call findWorkingCam starting from here
    int nextCameraIndex;

    /// Render image effect (simply renders a texture)
    Effects::RenderTexture rendertexture;
    Effects::Freeze random;

    /// Texture to hold input image
    Tucano::Texture* frameTexture;
    Eigen::Vector2i viewport;

    //FUNCTIONS
    int findWorkingCam(cv::VideoCapture** targetCamera, int starter);

protected:

    //events go in here 
    /**
     * @brief Callback for mouse press event.
     *
     * The mouse press starts a rotation or a translation if Shift is pressed.
     * @param event The mouse event that triggered the callback.
     */
    void mousePressEvent (QMouseEvent * event);

    /**
     * @brief Callback for mouse move event.
     *
     * If rotating or translating, this method updates the trackball position.
     * @param event The mouse event that triggered the callback.
     */
    void mouseMoveEvent (QMouseEvent * event);

    /**
     * @brief Callback for mouse release event.
     *
     * Stops rotation or translation.
     * @param event The mouse event that triggered the callback.
     */
    void mouseReleaseEvent (QMouseEvent * event);

    void keyPressEvent (QKeyEvent * key);

};

#endif // GLWIDGET
