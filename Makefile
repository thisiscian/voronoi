docker-build:
	docker build --tag=voronoi .

docker-run: build
	docker run -p 4000:80 voronoi

docker-build-dev:
	docker build --target base --tag=voronoi/base .
	docker build --target pybase --tag=voronoi/pybase .
	docker build --target dev --tag=voronoi/dev-base .
	docker build --tag=voronoi .

docker-test: docker-build-dev
	docker run --rm -e DISPLAY=${DISPLAY} voronoi build/run_voronoi.py

docker-edit: docker-build-dev
	docker create -it --name voronoi/dev voronoi/dev-base
	docker start voronoi/dev
	docker exec -it voronoi/dev bash
