docker-build:
	docker build --tag=voronoi .

docker-run: build
	docker run -p 4000:80 voronoi

docker-edit:
	docker create -it --name voronoi-dev voronoi
	docker start voronoi-dev
	docker exec -it voronoi-dev bash

