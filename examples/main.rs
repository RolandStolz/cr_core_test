use c_rust_module::Point;
use c_rust_module::Point_add;

fn main() {
    let p = Point { x: 1.0, y: 2.0 };
    println!("Point: ({}, {})", p.x, p.y);

    // Raw bindings require unsafe call
    let p2 = unsafe { Point_add(p, p) };
    println!("Point from unsafe: ({}, {})", p2.x, p2.y);

    // We can wrap the raw bindings in a function for point
    let p3 = p.add(&p);
    println!("Point from wrapper: ({}, {})", p3.x, p3.y);
}
