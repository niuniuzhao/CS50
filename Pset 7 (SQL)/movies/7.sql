-- List all movies released in 2010 and their ratings,
-- in descending order by rating
-- order them alphabetically by title.
-- output a table with two columns, one for the title of each movie and one for the rating of each movie.
-- Movies that do not have ratings should not be included in the result.

SELECT ratings.rating, movies.title
FROM ratings
JOIN movies ON ratings.movie_id = movies.id
WHERE year = 2010
ORDER BY rating DESC, title;
