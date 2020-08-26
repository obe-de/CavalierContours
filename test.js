const Module = require('./build/offset.js');

function offsetPolyline({ vertexes, isClosed }) {
  const cPline = Module.input_pline();

  vertexes.forEach(({ x, y, bulge }) => {
    cPline.vertexes.push_back({ x, y, bulge });
  });

  cPline.isClosed = isClosed;

  return Module.offset_polyline(cPline, 3);
}

Module.onRuntimeInitialized = () => {
  const result = offsetPolyline({ vertexes: [
    { x: 0, y: 25, bulge: 1 },
    { x: 0, y: 0, bulge: 0 },
    { x: 2, y: 0, bulge: 1 },
    { x: 10, y: 0, bulge: -0.5 },
    { x: 8, y: 9, bulge: 0.374794619217547 },
    { x: 21, y: 0, bulge: 0 },
    { x: 23, y: 0, bulge: 1 },
    { x: 32, y: 0, bulge: -0.5 },
    { x: 28, y: 0, bulge: 0.5 },
    { x: 39, y: 21, bulge: 0 },
    { x: 28, y: 12, bulge: 0 },
  ], isClosed: true }, 3);

  const plines = [];
  for (let i = 0; i < result.size(); i++) {
    const pline = result.get(i);

    const vertexes = [];
    for (let j = 0; j < pline.vertexes.size(); j ++) {
      vertexes.push(pline.vertexes.get(j));
    }

    pline.vertexes.delete();

    plines.push({
      vertexes,
      isClosed: pline.isClosed,
    })
  }

  result.delete();

  console.log('offset polyline:', JSON.stringify(plines, null, 2));
}
