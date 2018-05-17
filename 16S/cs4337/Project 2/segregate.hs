segregate :: [Int] -> [[Int]] 
segregate lst = [(evens lst)] ++ [(odds lst)]

evens :: [Int] -> [Int]
evens [] = []
evens (x:xs) = if (x `mod` 2) == 0 then x : (evens xs) else evens xs

odds :: [Int] -> [Int]
odds [] = []
odds (y:ys) = if (y `mod` 2) == 1 then y : (odds ys) else odds ys
