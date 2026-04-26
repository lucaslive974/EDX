const quad = document.querySelector('#quad');
const circle = document.querySelector('#circle');
const triangle = document.querySelector('#triangle');

let time = 0;
function rotateElement(element) {
  const interval = setInterval(() => {
    time += 0.01;
    element.style.transform = `rotate(${(time % 1) * 360}deg)`;
  }, 100);

  element.style.transform = 'rotate(180deg)';
}

rotateElement(quad);
rotateElement(triangle);
rotateElement(circle);
