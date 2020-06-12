using LinearAlgebra
using Plots

hadamard = 1im / sqrt(2) * [1 1; 1 -1]
tgate = [exp(1im*pi/8)  0; 0 exp(-1im*pi/8)]

seeds = Array{Complex{Float64}, 2}[hadamard, tgate]

gates = Array{Array{Complex{Float64}, 2}, 1}[seeds]

crosspair(x) = x[1] * x[2]

println("start generating")

for i in 1:16
    push!(gates,
        reshape(
            map(crosspair, 
                Iterators.product(gates[end], seeds)
            ),
            :
        )
    )
end

function vectorize(u::Array{Complex{Float64}, 2})::
    Array{Float64, 1}
    
    n = [imag(u[1, 2] + u[2, 1]) / 2.0,
         real(u[1, 2] - u[2, 1]) / 2.0,
         imag(u[2, 2] - u[1, 1]) / 2.0]

    cosTheta = real(u[1, 1] + u[2, 2]) / 2.0
    sinTheta = norm(n)

    theta = atan(sinTheta, cosTheta)
    if (norm(n) != 0)
        return normalize(n)
    else
        return n
    end
end

println("vectorizing gates");
r = map(vectorize, vcat(gates...));
x = [v[1] for v in r];
y = [v[2] for v in r];
z = [v[3] for v in r];

println("plotting");
gr();
