#version 330 core

#define BLUR_RARIUS 10


layout(pixel_center_integer) in vec4 gl_FragCoord;


uniform sampler2D	textureColor;


uniform	float	mipmapsCount;
uniform	vec2	screen;


out	vec4	oColor;


void main()
{
	vec2	screenTex = gl_FragCoord.xy/screen;

	oColor	= vec4(0.0f);

	
	for(int x = -BLUR_RARIUS; x <= BLUR_RARIUS; ++x) {
		for(int y = -BLUR_RARIUS; y <= BLUR_RARIUS; ++y) {
			float	factor = 1.0f - length(vec2(x, y)) / BLUR_RARIUS;
			
			if(factor > 0.0f) {
				vec2	tex = screenTex + vec2(x, y) / screen;
				// vec4	color = texture(textureColor, tex);
				vec4	color = textureLod(textureColor, tex, (1.0f - pow(factor, 2.0f)) * mipmapsCount);
				
				oColor.xyz	+= color.xyz * factor;
				oColor.w	+= factor;
			}
		}
	}
	

	oColor	= vec4(oColor.xyz / oColor.w, 1.0f);
}








