uniform vec2 center;
uniform float radius;
uniform vec4 color;
uniform float bleed;
uniform float linearFactor;
uniform bool outline;

void main() {

	vec2 pixel = gl_FragCoord.xy;
	
	float dist = length(center - pixel);
	
	float distFromFalloff = radius - dist;
	
	float attenuation = 0.0;
	attenuation = distFromFalloff * (bleed / pow(dist, 2) + linearFactor / radius);
			
	attenuation = clamp(attenuation, 0.0, 1.0);

	if(outline && floor(dist)==floor(radius)) attenuation = 1;
	
	vec4 color = vec4(attenuation, attenuation, attenuation, 1.0) * vec4(color.r, color.g, color.b, color.a);
	
	gl_FragColor = color;
}