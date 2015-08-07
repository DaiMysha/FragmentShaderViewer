uniform vec2 center;
uniform float radius;
uniform vec4 color;
uniform float directionAngle;
uniform float spreadAngle;
uniform float size;
uniform float bleed;
uniform float linearFactor;

float getAngleBetweenVectors(vec2 a, vec2 b) {
	return 0.0;
}

bool isPixelInLight() {
	vec2 pos = gl_FragCoord.xy;
	float angleFromOrigin = getAngleBetweenVectors(vec2(pos.x-center.x, pos.y-center.y),vec2(1.0-center.x,1.0-center.y));
	return true;
}

void main()
{
	float dist = length(center - gl_FragCoord.xy);
	
	float distFromFalloff = radius - dist;
	
	float attenuation = 0.0;
	attenuation = distFromFalloff * (bleed / pow(dist, 2.0) + linearFactor / radius);
			
	attenuation = clamp(attenuation, 0.0, 1.0);
	
	vec4 color = vec4(attenuation, attenuation, attenuation, 1.0) * vec4(color.r, color.g, color.b, 1.0);

	gl_FragColor = color;
}