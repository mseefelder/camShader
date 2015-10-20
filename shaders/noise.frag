#version 430

out vec4 out_Color;
uniform sampler2D frameTexture;
uniform ivec2 viewport;
uniform float scale;

uniform uint sem;

#define NBINS 16

float dist2(vec2 a, vec2 b)
{
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{	
	//vec2 texCoord = vec2(gl_FragCoord.xy/vec2(viewport.xy)) + scale*vec2(rand(gl_FragCoord.xy),rand(gl_FragCoord.yx));
  	//vec4 tex = texture(frameTexture, texCoord+vec2(0.01,0));
  	//out_Color = vec4(length(tex.xyz)*normalize(vec3(0.025,0.1,0.5)+tex.xyz), 1.0);
  	ivec2 position = ivec2(gl_FragCoord.xy);
  	vec4 px = texelFetch(frameTexture, position, 0);
  	out_Color = vec4(0.);
  	//if (px.r > scale)
  	//	out_Color = vec4(1.);
  	bool done = false;
    uint locked = 0;
    while(!done)
    {
     // locked = imageAtomicCompSwap(sem, coord, 0u, 1u); will NOT work
        locked = atomicExchange(sem, 1u);
        if (locked == 0)
        {
            //performYourCriticalSection();
            if (px.r > scale)
  				out_Color = vec4(1.);

            memoryBarrier();

            atomicExchange(sem, 0u);

            // replacing this with a break will NOT work
            done = true;
        }
    }
}
