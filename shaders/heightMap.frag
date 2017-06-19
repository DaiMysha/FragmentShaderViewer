uniform sampler2D heightMap;
uniform sampler2D lightMap;
uniform float width;
uniform float height;

//puts pic upside down
/*
void main() {
	vec2 pixel = gl_TexCoord[0];
	pixel.y = (height - pixel.y*height)/height;
	
	gl_FragColor = texture2D(heightMap, pixel);
}
//*/

/*
void main() {
	vec2 pixel = gl_TexCoord[0];
	pixel.y = (height - pixel.y*height)/height;
	
	pixel.y += texture2D(heightMap, pixel);
	
	gl_FragColor = vec4(pixel.x, pixel.y, 0, 1);
}
//*/

//*
void main() {
	vec2 pixel = gl_TexCoord[0];
	pixel.y = (height - pixel.y*height)/height;
	
	pixel.y += texture2D(heightMap, pixel);
	
	gl_FragColor = texture2D(lightMap, pixel);
}
//*/