uniform vec2 center;
uniform float radius;
uniform vec4 color;
uniform float bleed;
uniform float linearFactor;
uniform bool outline;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

void main() {

	vec2 pixel = gl_FragCoord.xy;
	vec2 delta = center - pixel;
	float dist = length(delta);
	delta /= dist;
	vec4 diffusepixel = texture2D(diffuseMap, gl_TexCoord[0].xy);
	vec4 normalpixel = texture2D(normalMap, gl_TexCoord[0].xy);
	vec3 N = normalize(normalpixel.rgb * 2.0 - 1.0 );
	float normalDot = max(dot(N, delta), 0.0);
	
	float distFromFalloff = radius - dist;
	
	float attenuation = 0.0;
	attenuation = distFromFalloff * (bleed / pow(dist, 2) + linearFactor / radius);
			
	attenuation = clamp(attenuation, 0.0, 1.0);	
	
	vec4 color = vec4(attenuation, attenuation, attenuation, 1.0) * color;
	
	gl_FragColor = color;
	gl_FragColor = color + color * normalDot;
	gl_FragColor = diffusepixel * color + color * normalDot;
}