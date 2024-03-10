const std = @import("std");

pub fn build(b: *std.Build) !void {

    // Build options

    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const with_range_coulomb = b.option(bool, "with_range_coulomb", "Range-separated Coulomb") orelse false;
    const with_polynomial_fit = b.option(bool, "with_polynomial_fit", "Polynomial fit") orelse false;
    const with_f12 = b.option(bool, "with_f12", "F12") orelse false;
    const with_4c1e = b.option(bool, "with_4c1e", "4C1E") orelse false;
    const pypzpx = b.option(bool, "pypzpx", "P orbitals convention (Py, Pz, Px)") orelse false;
    const min_expcutoff = b.option(f128, "min_expcutoff", "Minimal cutoff for integral screening") orelse null;
    const keep_going = b.option(bool, "keep going", "Do not trigger hard exit for numerical issues in Rys quadrature") orelse false;
    const with_cint2_interface = b.option(bool, "with_cint2_interface", "Enable old cint (version 2) interface") orelse true;

    // library

    const lib = b.addStaticLibrary(.{
        .name = "cint",
        .target = target,
        .optimize = optimize,
    });

    const user_config = b.addConfigHeader(.{
        .style = .{ .cmake = .{ .path = "src/config.h.in" } },
        .include_path = "config.h",
    }, .{});

    lib.addIncludePath(.{ .path = "include" });
    lib.addIncludePath(.{ .path = "src" });
    lib.addConfigHeader(user_config);

    lib.installHeadersDirectory("include", "");

    var flags = std.ArrayList([]const u8).init(b.allocator);
    defer flags.deinit();

    try flags.appendSlice(&.{
        "-ftree-vectorize",
        "-funroll-loops",
        "-fno-math-errno",
    });

    var sources = std.ArrayList([]const u8).init(b.allocator);
    defer sources.deinit();

    try sources.appendSlice(&.{
        "src/breit.c",
        "src/c2f.c",
        "src/cart2sph.c",
        "src/cint1e_a.c",
        "src/cint1e.c",
        "src/cint1e_grids.c",
        "src/cint2c2e.c",
        "src/cint2e.c",
        "src/cint3c1e_a.c",
        "src/cint3c1e.c",
        "src/cint3c2e.c",
        "src/cint_bas.c",
        "src/eigh.c",
        "src/fblas.c",
        "src/find_roots.c",
        "src/fmt.c",
        "src/g1e.c",
        "src/g1e_grids.c",
        "src/g2c2e.c",
        "src/g2e.c",
        "src/g3c1e.c",
        "src/g3c2e.c",
        "src/gout2e_simd.c",
        "src/misc.c",
        "src/optimizer.c",
        "src/polyfits.c",
        "src/rys_roots.c",
        "src/rys_wheeler.c",
        "src/sr_rys_polyfits.c",
        "src/autocode/breit1.c",
        "src/autocode/deriv3.c",
        "src/autocode/deriv4.c",
        "src/autocode/dkb.c",
        "src/autocode/gaunt1.c",
        "src/autocode/grad1.c",
        "src/autocode/grad2.c",
        "src/autocode/hess.c",
        "src/autocode/int1e_grids1.c",
        "src/autocode/int3c1e.c",
        "src/autocode/int3c2e.c",
        "src/autocode/intor1.c",
        "src/autocode/intor2.c",
        "src/autocode/intor3.c",
        "src/autocode/intor4.c",
        "src/autocode/lresc.c",
    });

    if (with_range_coulomb) {
        // lib.defineCMacro("WITH_RANGE_COULOMB", null);
    }

    if (with_polynomial_fit) {
        lib.defineCMacro("WITH_POLYNOMIAL_FIT", null);
    }

    if (with_f12) {
        lib.defineCMacro("WITH_F12", null);
        try sources.appendSlice(&.{
            "src/g2e_f12.c",
            "src/stg_roots.c",
            "src/cint2e_f12.c",
        });
    }

    if (with_4c1e) {
        lib.defineCMacro("WITH_4C1E", null);
        try sources.appendSlice(&.{
            "src/cint4c1e.c",
            "src/g4c1e.c",
        });
    }

    if (pypzpx) {
        lib.defineCMacro("PYPZPX", null);
    }

    if (min_expcutoff) |val| {
        const buf = try std.fmt.allocPrint(b.allocator, "{e}", .{val});
        defer b.allocator.free(buf);
        lib.defineCMacro("MIN_EXPCUTOFF", buf);
    }

    if (keep_going) {
        lib.defineCMacro("KEEP_GOING", null);
    }

    if (with_cint2_interface) {
        lib.defineCMacro("WITH_CINT2_INTERFACE", null);
    }

    lib.addCSourceFiles(.{
        .flags = flags.items,
        .files = sources.items,
    });

    lib.linkLibC();

    lib.linkSystemLibrary("m");
    lib.linkSystemLibrary("quadmath");

    b.installArtifact(lib);
}
