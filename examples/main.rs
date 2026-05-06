use c_rust_module::cr_c_bindings::Point;
use c_rust_module::cr_c_bindings::Point_add;

fn main() {
    let p = Point { x: 1.0, y: 2.0 };
    println!("Point: ({}, {})", p.x, p.y);

    let p2 = unsafe { Point_add(p, p) };
    println!("Point: ({}, {})", p2.x, p2.y);
}
