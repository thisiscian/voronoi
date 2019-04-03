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
	docker run --rm voronoi build/run_voronoi.py | feh 2>/dev/null

docker-edit: docker-build-dev
	docker run --rm -i --volume="/home/${USER}:/root/" voronoi/dev-base bash
