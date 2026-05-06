use crate::cr_c_bindings::{Point, Point_add};

impl Point {
    pub fn add(&self, other: &Point) -> Point {
        unsafe { Point_add(*self, *other) }
    }
}
