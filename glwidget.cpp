#include "glwidget.hpp"
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) : Tucano::QtPlainWidget(parent)
{
	initd = false;
	camera = NULL;
    nextCameraIndex = 0;
    maxCamIndex = 50;
}

GLWidget::~GLWidget()
{

}

void GLWidget::initialize (void)
{
	/// resize OpenGL viewport to current size
    glViewport(0, 0, this->width(), this->height());

    ///find and set camera to read from the first working webcam
    try
    {
    	nextCameraIndex = findWorkingCam(&camera, nextCameraIndex);
    }
    catch (exception& e)
    {
    	throw;
    }

    // the default is /shaders from your running dir
    string shaders_dir("../shaders/");

    rendertexture.setShadersDir(shaders_dir);
    rendertexture.initialize();
    random.setShadersDir(shaders_dir);
    random.initialize();


    //temporary
    std::cout<<"Reading frame..."<<std::endl;
    //*camera >> frame;
    camera->read(frame);
    std::cout<<"Frame has been read! Flipping..."<<std::endl;
	cv::flip(frame, frame, 0);
	std::cout<<"Frame has been flipped! Converting to texture..."<<std::endl;

	//frameTextureID = frameTexture.create(GL_TEXTURE_2D, GL_RGB, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
    frameTexture = new Tucano::Texture();
	frameTexture->create(GL_TEXTURE_2D, GL_RGB, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
	std::cout<<"Frame is now on the GPU!"<<std::endl;
    //temporary


    /// set this widget as initialized
    initd = true;
}

int GLWidget::findWorkingCam (cv::VideoCapture** targetCamera, int starter)
{
	//start searching for camera at given index
	int targetCameraIndex = starter;

	while(targetCameraIndex < maxCamIndex)
	{
		//reset camera with new index
		if(*targetCamera)
		{
			(*targetCamera)->release();
			delete camera;	
		}
		(*targetCamera) = new cv::VideoCapture(targetCameraIndex);

		//try to read a frame. If ->read() succeeds, it returns true and
		//the while loop is broken. If not, false is returned, continuing the loop
		cv::Mat scratch; 
		if ((*targetCamera)->read(scratch) != false)
		{
			std::cout<<"Found camera! \n w: "<<scratch.cols<<"; h: "<<scratch.rows<<std::endl;
            this->resizeGL(scratch.cols, scratch.rows);
			break;
		}

		//Increase camera index to continue search
		targetCameraIndex++;
	}
	//throw error if no camera is found
	if (targetCameraIndex == maxCamIndex)
	{
		cameraException camex;
		throw camex;
	}

	//returns the next index
	return targetCameraIndex++;
}

void GLWidget::paintGL (void)
{
	if(!initd)
        return;

	makeCurrent();

	glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //*camera >> frame;
    camera->read(frame);
	cv::flip(frame, frame, 0);
	frameTexture->create(GL_TEXTURE_2D, GL_RGB, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
	

	// renders the given image, not that we are setting a fixed viewport that follows the widgets size
    // so it may not be scaled correctly with the image's size (just to keep the example simple)
    Eigen::Vector2i viewport (this->width(), this->height());
    random.renderTexture(*frameTexture, viewport);//debug commented

    update();
}

void GLWidget::keyPressEvent (QKeyEvent * key){
    setFocus();
    int pressed = key->key();
    int modifiers = key->modifiers();

    switch(pressed)
    {
        case Qt::Key_Plus:
            random.incScale();
            break;
        case Qt::Key_Minus:
            random.decScale();
            break;
        case Qt::Key_F5:
            random.reloadShaders();
            break;
        default:
            QGLWidget::keyPressEvent(key);

    }

}

/**
* @brief Callback for mouse press event.
*
* The mouse press starts a rotation or a translation if Shift is pressed.
* @param event The mouse event that triggered the callback.
*/
void GLWidget::mousePressEvent (QMouseEvent * event)
{
    setFocus ();
    //markROI = true;
    
    //Eigen::Vector2f screen_pos (
    //    ((2*((float)event->x()/this->width()))-1.0)
    //    , 
    //    ((-2*((float)event->y()/this->height()))+1.0)
    //    );
    Eigen::Vector2i screen_pos (
        (event->x())
        , 
        abs(event->y()-this->height())
        );
}

/**
 * @brief Callback for mouse move event.
 *
 * If rotating or translating, this method updates the trackball position.
 * @param event The mouse event that triggered the callback.
 */
void GLWidget::mouseMoveEvent (QMouseEvent * event)
{
    //Eigen::Vector2f screen_pos (
    //    ((2*((float)event->x()/this->width()))-1.0)
    //    , 
    //    ((-2*((float)event->y()/this->height()))+1.0)
    //    );
    Eigen::Vector2i screen_pos (
        (event->x())
        , 
        abs(event->y()-this->height())
        );

}

/**
 * @brief Callback for mouse release event.
 *
 * Stops rotation or translation.
 * @param event The mouse event that triggered the callback.
 */
void GLWidget::mouseReleaseEvent (QMouseEvent * event)
{
    //Eigen::Vector2f screen_pos (
    //    ((2*((float)event->x()/this->width()))-1.0)
    //    , 
    //    ((-2*((float)event->y()/this->height()))+1.0)
    //    );
    Eigen::Vector2i screen_pos (
        (event->x())
        , 
        abs(event->y()-this->height())
        );

}

/*
http://www.stat.wisc.edu/~mchung/teaching/MIA/reading/diffusion.gaussian.kernel.pdf.pdf
*/