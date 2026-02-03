#include "mfem.hpp"
#include "fourdst/config/config.h"
#include "CLI/CLI.hpp"

#include <functional>

struct Options {
    std::string input_mesh = "stroid.mesh";
    std::string vishost = "localhost";
    int visport = 19916;
};

void ViewMesh(
    const std::string& vishost,
    int visport,
    const mfem::Mesh& mesh,
    mfem::GridFunction& gf,
    const std::string& title,
    bool first_step = true
) {
    mfem::socketstream sol_sock(vishost.c_str(), visport);
    if (!sol_sock.is_open()) {
        std::cerr << "Unable to connect to GLVis server at "
                  << vishost << ':' << visport << std::endl;
        std::exit(1);
    }

    sol_sock.precision(8);
    sol_sock << "solution\n" << mesh << gf;
    if (first_step) {
        sol_sock << "window_title '" << title << "'\n";
        sol_sock << "keys 'iIzzMaagpmtppc'";
    }
    sol_sock << std::flush;
}

std::unique_ptr<mfem::Mesh> load_mesh(const std::string& filename) {
    return std::make_unique<mfem::Mesh>(filename.c_str());
}

void project_radial_function(mfem::GridFunction& gf, std::function<double(const mfem::Vector& x)> g) {
    mfem::FunctionCoefficient density_coeff([g](const mfem::Vector& x) {
        return g(x);
    });

    gf.ProjectCoefficient(density_coeff);
}



int main(const int argc, char** argv) {
    CLI::App app("MFEM_EXAMPLE");

    fourdst::config::Config<Options> config;
    fourdst::config::register_as_cli(config, app);
    CLI11_PARSE(app, argc, argv);

    auto mesh = load_mesh(config->input_mesh);

    const mfem::H1_FECollection fec(1, mesh->Dimension());
    mfem::FiniteElementSpace fes(mesh.get(), &fec);
    mfem::GridFunction rho_gf(&fes);
}
