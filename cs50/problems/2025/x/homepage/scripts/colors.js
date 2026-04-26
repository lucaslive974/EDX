let redColor = 255;
let greenColor = 0;
let blueColor = 0;
let alphaColor = 1;

const redInput = document.getElementById('red-range');
const greenInput = document.getElementById('green-range');
const blueInput = document.getElementById('blue-range');
const alphaInput = document.getElementById('alpha-range');


function updateColor() {
    redColor = redInput.value;
    greenColor = greenInput.value;
    blueColor = blueInput.value;
    alphaColor = alphaInput.value;

    const colorDisplay = document.getElementById('color-display');
    colorDisplay.style.backgroundColor = `rgba(${redColor}, ${greenColor}, ${blueColor}, ${alphaColor})`;
}


redInput.addEventListener("change", updateColor);
greenInput.addEventListener("change", updateColor);
blueInput.addEventListener("change", updateColor);
alphaInput.addEventListener("change", updateColor);
