#ifndef __FREEZE__
#define __FREEZE___

#include <tucano.hpp>

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
        bool hasFbo = false;
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
        bool hasFbo = false;

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
        noise.setUniform("frameTexture", tex.bind());
        noise.setUniform("viewport", viewport);
        noise.setUniform("scale", scale);
        quad.render();

        noise.unbind();
        tex.unbind();

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
};

}



#endif

