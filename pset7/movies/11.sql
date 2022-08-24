SELECT title
FROM movies JOIN stars ON movies.id = stars.movie_id JOIN people ON people.id = stars.person_id JOIN ratings on movies.id = ratings.movie_id -- Join the 4 tables together by id and sorted by rating in descending order
WHERE name = "Chadwick Boseman"
ORDER BY rating DESC LIMIT 5;

