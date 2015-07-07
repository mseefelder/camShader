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

	ivec2 intPos = ivec2(gl_FragCoord.xy);
	
	vec3 self = texelFetch(frameTexture, intPos, 0).rgb;
	vec3 NE = texelFetch(frameTexture, intPos+ivec2(1,1), 0).rgb;
	vec3 NW = texelFetch(frameTexture, intPos+ivec2(-1,1), 0).rgb;
	vec3 SE = texelFetch(frameTexture, intPos-ivec2(-1,1), 0).rgb;
	vec3 SW = texelFetch(frameTexture, intPos-ivec2(1,1), 0).rgb;

	float updown = length(NW-SE)+length(self-NW)+length(self-SE);
	float downup = length(SW-NE)+length(self-SW)+length(self-NE);

	if(updown > downup)
		out_Color = vec4((SW+NE)/2.0, 1.0);
	else
		out_Color = vec4((NW+SE)/2.0, 1.0);
}
