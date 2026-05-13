use crate::{
    PointArray,
    cr_c_bindings::{Point, State},
    cr_c_bindings::{Point_add, PointArray_clear, PointArray_pop, PointArray_push},
};

impl Point {
    pub fn new(x: f64, y: f64) -> Self {
        println!("Rust point constructor");
        Self { x, y }
    }

    pub fn add(&self, other: &Point) -> Point {
        unsafe { Point_add(*self, *other) }
    }
}

impl State {
    pub fn new(position: Point, orientation: f64, velocity: f64, time: usize) -> Self {
        println!("Rust state constructor");
        Self {
            position,
            orientation,
            velocity,
            time,
        }
    }
}

impl PointArray {
    pub fn len(&self) -> usize {
        self.count
    }

    pub fn as_slice(&self) -> &[Point] {
        unsafe { std::slice::from_raw_parts(self.data, self.count) }
    }

    pub fn as_mut_slice(&mut self) -> &mut [Point] {
        unsafe { std::slice::from_raw_parts_mut(self.data, self.count) }
    }

    pub fn push(&mut self, p: Point) -> bool {
        unsafe { PointArray_push(self, p) }
    }

    pub fn pop(&mut self) -> Option<Point> {
        unsafe {
            let mut out = Point { x: 0.0, y: 0.0 };
            PointArray_pop(self, &mut out);
            Some(out)
        }
    }

    pub fn clear(&mut self) {
        unsafe { PointArray_clear(self) }
    }
}
