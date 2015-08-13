uniform vec2 center;
uniform float radius;
uniform vec4 color;
uniform float bleed;
uniform float linearFactor;
uniform bool outline;
uniform bool iso;

uniform float thresholdCount;

void main() {

	vec2 pixel = gl_FragCoord.xy;
	if(iso) pixel.y = center.y + 2*(pixel.y - center.y);
	float dist = length(center - pixel);
	
	float distFromFalloff = radius - dist;
	
	float attenuation = 0.0;
	attenuation = distFromFalloff * (bleed / pow(dist, 2) + linearFactor / radius);
			
	attenuation = clamp(attenuation, 0.0, 1.0);

	if(outline && floor(dist)==floor(radius)) attenuation = 1;
	
	float newAtt = 0.0f;
	
	for(float i=0;i<thresholdCount;++i) {
		if(attenuation > (i) / thresholdCount) {
			newAtt = (i) / thresholdCount;
		}
	}

	vec4 color = vec4(newAtt, newAtt, newAtt, 1.0) * vec4(color.r, color.g, color.b, color.a);
	
	gl_FragColor = color;
}