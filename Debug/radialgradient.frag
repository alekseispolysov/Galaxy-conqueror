uniform vec2 resolution; // Size of the circle
uniform vec2 center;     // Center of the circle
uniform vec3 color;      // Base color (RGB)
uniform float radius;    // Radius of the circle

void main()
{
    vec2 position = gl_FragCoord.xy; // Current pixel position
    float dist = distance(position, center); // Distance from the circle's center

    if (dist > radius) discard; // Pixels outside the circle are transparent

    float factor = 1.0 - dist / radius; // Gradient factor (0 at edge, 1 at center)
    vec3 gradientColor = color * factor; // Interpolate color

    gl_FragColor = vec4(gradientColor, 1.0); // Final color
}
