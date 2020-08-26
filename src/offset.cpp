#include "cavaliercontours.h"
#include "cavc/polylinecombine.hpp"
#include "cavc/polylineoffset.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <emscripten/bind.h>
using namespace emscripten;

struct Vertex {
  double x;
  double y;
  double bulge;
};

Vertex toVertex(cavc::PlineVertex<double> pV) {
  Vertex v;
  v.x = pV.x();
  v.y = pV.y();
  v.bulge = pV.bulge();
  return v;
}

struct Polyline {
  std::vector<Vertex> vertexes;
  bool isClosed;
};

Polyline toPolyline(cavc::Polyline<double> pL) {
  Polyline p;

  auto vertexes = pL.vertexes();
  std::transform(vertexes.begin(), vertexes.end(), std::back_inserter(p.vertexes), toVertex);
  p.isClosed = pL.isClosed();

  return p;
}

std::vector<Polyline> offset_polyline(Polyline plineInput, double delta) {
  cavc::Polyline<double> input;

  for (auto i: plineInput.vertexes) {
    input.addVertex(i.x, i.y, i.bulge);
  }
  input.isClosed() = plineInput.isClosed;

  auto plines = cavc::parallelOffset(input, delta);

  std::vector<Polyline> jsPlines;
  std::transform(plines.begin(), plines.end(), std::back_inserter(jsPlines), toPolyline);

  return jsPlines;
}

Polyline input_pline() {
  Polyline p;
  return p;
}

EMSCRIPTEN_BINDINGS(my_module) {
  function("offset_polyline", &offset_polyline);
  function("input_pline", &input_pline);
  
  value_object<Vertex>("Vertex")
    .field("x", &Vertex::x)
    .field("y", &Vertex::y)
    .field("bulge", &Vertex::bulge);
  
  value_object<Polyline>("Polyline")
    .field("vertexes", &Polyline::vertexes)
    .field("isClosed", &Polyline::isClosed);

  register_vector<Vertex>("vector<Vertex>");
  register_vector<Polyline>("vector<Polyline>");
}
