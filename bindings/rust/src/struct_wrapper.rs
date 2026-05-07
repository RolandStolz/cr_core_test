use crate::cr_c_bindings::{Point, Point_add};

impl Point {
    pub fn new(x: f64, y: f64) -> Self {
        println!("Point constructor");
        Self { x, y }
    }

    pub fn add(&self, other: &Point) -> Point {
        unsafe { Point_add(*self, *other) }
    }
}
