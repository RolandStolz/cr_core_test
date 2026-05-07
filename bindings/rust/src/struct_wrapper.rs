use crate::cr_c_bindings::{Point, Point_add, State};

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
