uniform vec2 center;     // Center of the circle
uniform vec3 color;      // Base color (RGB)
uniform float radius;    // Radius of the circle
uniform float windowHeight;    // windows height
uniform float windowWidth;
uniform vec2 viewOffset; // By default shader is affected by window coordinates, this view Offset applying moving of camera to shaders
uniform float zoomFactor;
uniform vec2 viewSize; // size of the 
uniform vec2 screenSize;

void main()
{
    vec2 shaderPos = gl_FragCoord.xy;

    // (shaderPos) +
    vec2 position =  (shaderPos) + viewOffset; // Current pixel position + adjustments to shaders
    // Invert the y-coordinate to match SFML's coordinate system (origin at top-left)
    
    //position.x = windowWidth --- Inversing position
    position.y = windowHeight - position.y;
    
    position = position;

    vec2 newCenter = center;
    //newCenter.y = -center.y;

    // logic: if zoom > 1 -> move center away from center of vec away from center, but if one of center ordinates is sitting on middle of width or height of screen in this case don't move them
    // same for zoom < 1, but move everything closer to center.
    // if it is one -> don't do anything

    float viewWidth = viewSize.x;
    float viewHeight = viewSize.y;

    

    if (zoomFactor != 1.0) {
        vec2 screenCenter = vec2(viewOffset.x, -viewOffset.y) + screenSize / 2.0;
        vec2 direction = center - screenCenter;
        newCenter = screenCenter + direction * zoomFactor;
    }


    float dist = distance(position, newCenter)   ; // Distance from the circle's center float dist = distance(position, center) 

    float adjustedRadius = radius * zoomFactor;

    if (dist > adjustedRadius) discard; // Pixels outside the circle are transparent 

    // Calculate fade factor: brightest at center, black at edge
    float factor = 1.0 - (dist / adjustedRadius) ; // Normalize fade from center to edge - (dist / adjustedRadius)
    factor = clamp(factor, 0.0, 1.0);     // Ensure factor stays in range [0, 1]
    factor = smoothstep(0.0, 0.85, factor);  // Optional: smooth fade
    //factor = max(factor, 0.95); // Force center to be at least 0.95 alpha

    // Interpolate the color
    vec3 gradientColor = color; // if I want to change it back to vect free and constant vel. I will just add * factor and replace factor in gl_fragColor to 1.0

    // Set the final pixel color
    // use wordl coords in my calc
    gl_FragColor = vec4(gradientColor, factor);
}