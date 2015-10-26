#ifndef __FREEZE__
#define __FREEZE___

#include <tucano.hpp>
#include <ctime>

#define NBINS 16

namespace Effects
{

/**
 * @brief Picks a 3D position from screen position
 */
class Freeze : public Tucano::Effect
{

public:

    /**
     * @brief Default constructor.
     */
    Freeze ()
    {
    }

    /**
     * @brief Default destructor
     */
    virtual ~Freeze (void ) {}

    /**
     * @brief Initializes the effect, creating and loading the shader.
     */
    virtual void initialize()
    {
        sem = new ShaderStorageBufferInt(1);
        framecount = 0;
        firstTime = false;

        loadShader(noise, "noise");
        quad.createQuad();
    }

    /**
     * @brief Renders the given texture.
     *
     * Renders the given texture using a proxy geometry, a quad the size of the viewport
     * to hold the texture.
     */
    void renderTexture (Texture& tex, Eigen::Vector2i viewport)
    {
        glViewport(0, 0, viewport[0], viewport[1]);

        noise.bind();
        sem->bindBase(0);
        //noise.setUniform("frameTexture", tex.bind());
        noise.setUniform("viewport", viewport);
        noise.setUniform("scale", scale);
        //GLint location = noise.getUniformLocation("sem");
        //glUniform1ui(location, sem);//setUniform("sem", sem);
        quad.render();

        noise.unbind();
        tex.unbind();
        sem->unbindBase();

        if (framecount++ > 10000)
        {
            if (firstTime)
            {
                std::time_t beforeLast = last;
                std::time(&last);
                std::cout<<(1.0*framecount)/difftime(last, beforeLast)<<" fps"<<std::endl;    
            }
            else
            {
                std::time(&last);
                firstTime = true;
            }
            framecount = 0;
        }

    }

    void incScale(){
        scale += 0.01;
    }

    void decScale(){
        scale -= 0.01;
    }

private:

    /// The mean filter shader.
    Shader noise;

    /// A quad to be rendered forcing one call of the fragment shader per image pixel (its just a proxy geometry)
    Mesh quad;

    ///
    float scale;

    int framecount;

    std::time_t last;

    bool firstTime;

    //unsigned int sem;

    ShaderStorageBufferInt* sem;
};

}



#endif

