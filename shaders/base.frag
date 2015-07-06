#version 430

out vec4 out_Color;
uniform sampler2D frameTexture;
uniform ivec2 viewport;

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
	vec2 texCoord = vec2(gl_FragCoord.xy/vec2(viewport.xy)) + 0.01*vec2(rand(gl_FragCoord.xy));
  	vec4 tex = texture(frameTexture, texCoord+vec2(0.01,0));
  	out_Color = vec4(length(tex.xyz)*normalize(vec3(0.025,0.1,0.5)+tex.xyz), 1.0);
}
