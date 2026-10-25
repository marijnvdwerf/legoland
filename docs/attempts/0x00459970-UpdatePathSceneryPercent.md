# UpdatePathSceneryPercent (0x00459970)

Current full verify and harness baseline: **94.83%**.

The function already has the same instruction count as the original. Its
diff contains only scheduling differences for the cx/cy stores and
rectangle-bound loads around CountPathEdgeScenery calls. No source-shape
trial was justified in this pass; retained the source and annotated its
periodic perimeter-counting behavior.
