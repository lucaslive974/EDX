let canvas = document.getElementById("gl-canvas");
let gl = canvas.getContext("webgl");

if (!gl) {
    alert("WebGL not supported, falling back on experimental-webgl");
    gl = canvas.getContext("experimental-webgl");
}

if (!gl) {
    alert("Your browser does not support WebGL");
}

gl.clearColor(1.0, 1.0, 1.0, 1.0);

gl.clear(gl.COLOR_BUFFER_BIT);

gl.viewport(0, 0, canvas.width, canvas.height);


let vertexShaderSource = `
attribute vec4 a_position;
void main() {
    gl_Position = a_position;
}
`;

let fragmentShaderSource = `
void main() {
    gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0); // Red color
}
`;

function createShader(gl, type, source) {
    let shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);

    if (gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        return shader;
    } else {
        console.error("An error occurred compiling the shaders: " + gl.getShaderInfoLog(shader));
        gl.deleteShader(shader);
        return null;
    }
}

function createProgram(gl, vertexShader, fragmentShader) {
    let program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);

    if (gl.getProgramParameter(program, gl.LINK_STATUS)) {
        return program;
    } else {
        console.error("Unable to initialize the shader program: " + gl.getProgramInfoLog(program));
        return null;
    }
}

let vertexShader = createShader(gl, gl.VERTEX_SHADER, vertexShaderSource);
let fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentShaderSource);

let shaderProgram = createProgram(gl, vertexShader, fragmentShader);

if (!shaderProgram) {
    console.error("Failed to create shader program");
}
gl.useProgram(shaderProgram);
let positionAttributeLocation = gl.getAttribLocation(shaderProgram, "a_position");
let positionBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

let positions = new Float32Array([
    -0.5, -0.5,
     0.5, -0.5,
     0.5,  0.5,
    -0.5,  0.5,
]);


gl.bufferData(gl.ARRAY_BUFFER, positions, gl.STATIC_DRAW);
gl.enableVertexAttribArray(positionAttributeLocation);
gl.vertexAttribPointer(positionAttributeLocation, 2, gl.FLOAT, false, 0, 0);

// Draw the triangle
gl.clear(gl.COLOR_BUFFER_BIT);
gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
