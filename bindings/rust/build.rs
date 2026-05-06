fn main() {
    cc::Build::new()
        .files(["src/functions.c", "src/structs.c"])
        .include("src")
        .compile("cr_lib");

    let bindings_c = bindgen::Builder::default()
        .header("src/structs.h")
        .header("src/functions.h")
        .clang_arg("-Isrc")
        // Classes
        .allowlist_type("Point")
        // Functions
        .allowlist_function("Point_add")
        .generate()
        .expect("Unable to generate bindings");

    bindings_c
        .write_to_file("bindings/rust/src/cr_c_bindings.rs")
        .expect("Couldn't write C bindings!");
}
