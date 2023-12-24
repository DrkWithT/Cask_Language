# Cask Samples

### Simple Aggregate
```
    # 2d point structure!
    agg Point {
        int x
        int y
    }

    origin : Point = {0, 0}
```

### Simple Function
```
    # Does a linear search on an array of int values.
    func search_int(numbers : int[], item : int)
        count : int = length(numbers)
        index : int = 0

        while (index < count)
            if (numbers[index] == item)
                return index
            end

            index = index + 1
        end
    end

    return -1
```

### Recursive (divide and conquer) Function
```
    # Finds a^n by recursive divide and conquer strategy.
    func dq_pow(base : int , pow : int)
        if (pow == 0)
            return 1
        end

        if (pow == 1)
            return base
        end

        even_p : int = pow / 2
        odd_p : int = (pow - 1) / 2

        if (is_even(pow) == true)
            return dq_pow(base, even_p) * dq_pow(base, even_p)
        else
            return base * dq_pow(base, odd_p) * dq_pow(base, odd_p)
        end
    end
```
