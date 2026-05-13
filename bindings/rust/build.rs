fn main() {

    let c_src_files = vec!["src/functions.c", "src/structs.c"];

    cc::Build::new()
        .files(&c_src_files)
        .include("src")
        .compile("cr_lib");

    let bindings_c = bindgen::Builder::default()
        .header("src/structs.h")
        .header("src/functions.h")
        .clang_arg("-Isrc")
        // Classes
        .allowlist_type("Point")
        .allowlist_function("PointArray_.*")
        .allowlist_type("State")
        .allowlist_type("Lanelet")
        // Functions
        .allowlist_function("Point_add")
        .generate()
        .expect("Unable to generate bindings");

    bindings_c
        .write_to_file("bindings/rust/src/cr_c_bindings.rs")
        .expect("Couldn't write C bindings!");

    // Needs to be done for headers as well
    for file in &c_src_files {
        println!("cargo:rerun-if-changed={}", file);
    }
}
