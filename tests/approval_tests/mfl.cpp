#include "framework/approve.hpp"
#include "framework/doctest.hpp"
#include "renderer/render_formulas.hpp"

#include <iostream>

namespace mfl
{
    TEST_CASE("mfl")
    {
        std::vector<std::string> formulas = {
            R"(M(s)<M(t)<|M| = m \qquad y'' = c\{f[y',y(x)] + g(x)\})",
            R"(a = \sqrt[2]{b^2 + c^2} \qquad \sqrt[n]{\frac{x^n - y^n}{1 + u^{2n}}} \qquad \sqrt[3]{-q + \sqrt{q^2 + p^3}})",
            R"(A^{x_i^2}_{j^{2n}_{n,m}} \qquad \Gamma(x) \equiv \lim_{n \rightarrow \infty} \prod_{v=0}^{n-1} \frac{n!n^{x-1}}{x+v} \equiv \int^\infty_0 e^{-t}t^{x-1} \, dt)",
            R"(\widehat{1-x} \not= \widehat{-y} \not\approx \widetilde{xyz} \qquad \overline{\overline{a}^2 + \underline{xy} + \overline{\overline{z}}})",
            R"(\lim_{x \rightarrow 0} \frac{\ln \sin \pi x}{\ln \sin x} = \lim_{x \rightarrow 0} \frac{\pi \tan x}{\tan \pi x})",
            R"(\left[ \int + \int \right]_{x=0}^{x=1})",
            R"(\mathit{differ}\ne\mathnormal{differ}=differ \qquad \mathbb{ABCabc123} \mathfrak{XYZxyz890})",
            R"(M\hat{}M)",
            R"(\{x|x^2∈ℝ\})",
            R"(\sum\nolimits_{i=0}^n x_i  \qquad \oint\limits_0^\infty \qquad \oiiint\limits_0^1)",
        };

        const auto result = render_formulas({.width = 600_px,
                                             .height = 800_px,
                                             .columns = {{.initial_offset = 50_px, .line_height = 50_px, .x = 100_px}}},
                                            formulas);
        approve(result);
    }

    TEST_CASE("mfl_large")
    {
        std::vector<std::string> formulas = {
            R"(M(s)<M(t)<|M| = m)",
            R"(y'' = c\{f[y',y(x)] + g(x)\})",
            R"(A^{x_i^2}_{j^{2n}_{n,m}})",
            R"(\Gamma(x) \equiv \lim_{n \rightarrow \infty} \prod_{v=0}^{n-1} \frac{n!n^{x-1}}{x+v})",
        };

        const auto result = render_formulas({.width = 600_px,
                                             .height = 800_px,
                                             .font_size = 20_pt,
                                             .columns = {{.initial_offset = 50_px, .line_height = 100_px, .x = 10_px}}},
                                            formulas);
        approve(result);
    }

    TEST_CASE("mathtext")
    {
        std::vector<std::string> formulas = {
            R"(a+b+\dot s+\dot{s}+\ldots)",
            R"(x \doteq y)",
            R"(\frac{\$100.00}{y})",
            R"(x   y)",
            R"(x+y\ x=y\ x<y\ x:y\ x,y\ x@y)",
            R"(100\%y\ x*y\ x/y x\$y)",
            R"(x\leftarrow y\ x\forall y\ x-y)",
            R"(x \mathsf x \mathbf x \mathcal X \mathrm x)",
            R"(x\ x\,x\;x\quad x\qquad x\!x\hspace{ 0.5 }y)",
            R"(\{ \mathrm{braces} \})",
            R"(\qquad \qquad \left[\left\lfloor\frac{5}{\frac{\left(3\right)}{4}} y\right)\right])",
            R"(\left(x\right))",
            R"(\sin(x))",
            R"(x_2 \quad x^2 \quad x^2_y \quad x_y^2)",
            R"(\prod_{i=\alpha_{i+1}}^\infty)",
            R"(x = \frac{x+\frac{5}{2}}{\frac{y+3}{8}})",
            R"(dz/dt = \gamma x^2 + \mathrm{sin}(2\pi y+\phi))",
            R"(\alpha_{i+1}^j = \mathrm{sin}(2\pi f_j t_i) e^{-5 t_i/\tau})",
            R"(\mathcal{R}\prod_{i=\alpha_{i+1}}^\infty a_i \sin(2 \pi f x_i))",
            R"(\Delta_i^j \qquad \Delta^j_{i+1})",
            R"(\ddot{o}\acute{e}\grave{e}\hat{O}\breve{\imath}\tilde{n}\vec{q})",
            R"(\arccos((x^i)))",
            R"(\gamma = \frac{x=\frac{6}{8}}{y} \delta)",
            R"(\limsup_{x\to\infty} \qquad\qquad\qquad \oint^\infty_0)",
            R"(f'\quad f'''(x)\quad ''/\mathrm{yr})",
            R"(\frac{x_2888}{y})",
            R"(\qquad\qquad\sqrt[3]{\frac{X_2}{Y}}=5)",
            R"(\sqrt[5]{\prod^\frac{x}{2\pi^2}_\infty})",
            R"(\qquad\qquad\sqrt[3]{x}=5)",
            R"(\frac{X}{\frac{X}{Y}})",
            R"(W^{3\beta}_{\delta_1 \rho_1 \sigma_2} = U^{3\beta}_{\delta_1 \rho_1} + \frac{1}{8 \pi 2} \int^{\alpha_2}_{\alpha_2} d \alpha^\prime_2 \left[\frac{ U^{2\beta}_{\delta_1 \rho_1} - \alpha^\prime_2U^{1\beta}_{\rho_1 \sigma_2} }{U^{0\beta}_{\rho_1 \sigma_2}}\right])",
            R"(\mathcal{H} = \int d \tau \left(\epsilon E^2 + \mu H^2\right))",
            R"(\widehat{abc}\widetilde{def})",
            R"(\Gamma \Delta \Theta \Lambda \Xi \Pi \Sigma \Upsilon \Phi \Psi \Omega)",
            R"(\alpha \beta \gamma \delta \epsilon \zeta \eta \theta \iota \lambda \mu \nu \xi \pi \kappa \rho \sigma \tau \upsilon \phi \chi \psi)",
        };

        const auto result = render_formulas({.width = 1000_px,
                                             .height = 800_px,
                                             .columns = {{.line_height = 35_px, .x = 10_px, .num_rows = 22},
                                                         {.line_height = 45_px, .x = 410_px, .num_rows = 22}}},
                                            formulas);

        approve(result);
    }

    TEST_CASE("MathML_torture_test")
    {
        std::vector<std::string> formulas = {
            R"({x}^{2}{y}^{2})",
            R"({}_{2}F_{3})",
            R"(\frac{x+{y}^{2}}{k+1})",
            R"(x+{y}^{\frac{2}{k+1}})",
            R"(\frac{a}{b/2})",
            R"({a}_{0}+\frac{1}{{a}_{1}+\frac{1}{{a}_{2}+\frac{1}{{a}_{3}+\frac{1}{{a}_{4}}}}})",
            R"(\binom{n}{k/2})",
            R"(\binom{p}{2}{x}^{2}{y}^{p-2}-\frac{1}{1-x}\frac{1}{1-{x}^{2}})",
            R"(\sum _{i=1}^{p}\sum _{j=1}^{q}\sum _{k=1}^{r}{a}_{ij}{b}_{jk}{c}_{ki})",
            R"({x}^{2y})",
            R"(\sqrt{1+\sqrt{1+\sqrt{1+\sqrt{1+\sqrt{1+\sqrt{1+\sqrt{1+x}}}}}}})",
            R"(\left(\frac{{\partial }^{2}}{\partial {x}^{2}}+\frac{{\partial }^{2}}{\partial {y}^{2}}\right){|\varphi \left(x+iy\right)|}^{2}=0)",
            R"({2}^{{2}^{{2}^{x}}})",
            R"({\int }_{1}^{x}\frac{\mathrm{dt}}{t})",
            R"(\int {\int }_{D}\mathrm{dx} \mathrm{dy})",
            R"({y}_{{x}_{2}})",
            R"({x}_{92}^{31415}+\pi)",
            R"({x}_{{y}_{b}^{a}}^{{z}_{c}^{d}})",
            R"({y}_{3}^{\prime \prime \prime })",
            R"(\left( \xi \left( 1 - \xi \right) \right))",
            R"(\left(2 \, a=b\right))",
            R"(? ! &)",
            R"(\operatorname{cos} x)",
            R"(\sum _{\genfrac{}{}{0}{}{0\leq i\leq m}{0<j<n}}P\left(i,j\right))",
            R"(\left\Vert a \right\Vert \left\vert b \right\vert \left| a \right| \left\| b\right\| \Vert a \Vert \vert b \vert)",
            R"(\mathring{A}  \AA)",
            R"(M \, M \, M \/ M \> M \: M \; M \ M \enspace M \quad M \qquad M \! M)",
            R"(\Cup \, \Cap \, \leftharpoonup \, \barwedge \, \rightharpoonup)",
            R"(\dotplus \, \doteq \, \doteqdot \, \ddots)",
            R"(xyz^kx_kx^py^{p-2} d_i^jb_jc_kd x^j_i E^0 E^0_u)",
            R"({xyz}^k{x}_{k}{x}^{p}{y}^{p-2} {d}_{i}^{j}{b}_{j}{c}_{k}{d} {x}^{j}_{i}{E}^{0}{E}^0_u)",
            R"({\int}_x^x x\oint_x^x x\int_{X}^{X}x\int_x x \int^x x \int_{x} x\int^{x}{\int}_{x} x{\int}^{x}_{x}x)",
            R"(\overline{\omega}^x \frac{1}{2}_0^x)",
            R"(\left(X\right)_{a}^{b})",
        };

        const auto result = render_formulas({.width = 1200_px,
                                             .height = 1000_px,
                                             .columns = {{.line_height = 60_px, .x = 10_px, .num_rows = 17},
                                                         {.line_height = 60_px, .x = 410_px, .num_rows = 17}}},
                                            formulas);

        approve(result);
    }
}