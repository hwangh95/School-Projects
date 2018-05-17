zipper :: [a] -> [a] -> [[a]]
zipper _ [] = []
zipper [] _ = []
zipper (x:xs) (y:ys) = [x,y] : (zipper xs ys)
