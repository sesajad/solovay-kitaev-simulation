using Distributed
using LinearAlgebra
using Statistics
using Plots

hadamard = 1im / sqrt(2) * [1 1; 1 -1];
tgate = [exp(1im*pi/8)  0; 0 exp(-1im*pi/8)];

seeds = Array{Complex{Float64}, 2}[hadamard, tgate];

gates = Array{Array{Complex{Float64}, 2}, 1}[seeds];

crosspair(x) = x[1] * x[2];

println("start generating");

for i in 1:20
    println("\t ", i, " level gates");
    push!(gates,
        reshape(
            map(crosspair, 
                Iterators.product(gates[end], seeds)
            ),
            :
        )
    )
end

struct Repr
    theta:: Float64
    n:: Array{Float64, 1}
end

function vectorize(u::Array{Complex{Float64}, 2})::Repr
    n = [imag(u[1, 2] + u[2, 1]) / 2.0,
         real(u[1, 2] - u[2, 1]) / 2.0,
         imag(u[2, 2] - u[1, 1]) / 2.0]

    cosTheta = real(u[1, 1] + u[2, 2]) / 2.0
    sinTheta = norm(n)

    theta = atan(sinTheta, cosTheta)
    if (norm(n) != 0)
        normalize!(n)
    end

    Repr(theta, n)
end

println("vectorizing gates");
vectors = map(gs -> map(vectorize, gs), gates);

function randomSU2()::Array{Complex{Float64}, 2}
    # sorry to implement this!
    vec = randn((4,))
    while (norm(vec) == 0)
        vec = randn((4,))
    end
    a, b, c, d = normalize(vec)
    return [(a + im * b) (-c + im * d); (c + im * d) (a - im * b)]
end

epsilon(a::Repr, b::Repr)::Float64 = 2 * (1 - cos(a.theta) * cos(b.theta) -
    sin(a.theta) * sin(b.theta) * dot(a.n, b.n))

epsilonStar(u::Repr, gates::Array{Repr, 1})::Float64 = minimum(map(x -> epsilon(u, x), gates))

#meanEpsilonStar(samples::Array{Repr, 1}, gates::Array{Repr, 1})::Float64 = mean(x -> epsilonStar(x, gates), samples)

println("generating samples");
samples = [vectorize(randomSU2()) for _ in 1:100];

println("finding min errors");
values = [[-log(epsilonStar(x, vecs)) for x in samples] 
    for vecs in vectors];


println("writing to file");
open("log-errors.txt", "w") do f
    for vs in values
        write(f, join(vs, ","));
        write(f, "\n");
    end
end
