local x = os.clock()

function loop(n)
    local count = 0

    for i = 1, n do
        count = count + 1
    end
    return count
end

local N = 10 * 10000000

print(loop(N))

print(string.format("elapsed time: %.2f\n", os.clock() - x))