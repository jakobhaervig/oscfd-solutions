import os
import numpy as np
from OpenFoamDataWriter import OpenFoamDataWriter


class GenerateRepeatingStep(object):
    def __init__(self):
        self.n_steps = 3

        self.L1 = 3  # Length from inlet to start of step (cm)
        self.L2 = 4  # Length from inlet to end of step (cm)
        self.L3 = 7  # Length from inlet to outlet (cm)
        self.h = 0.5  # Height of step (cm)
        self.H = 2  # Height of channel (cm)
        self.s = 1  # Depth of channel (cm)

        self.mesh_den = 10  # Number of cells per cm

    def _create_one_level_vertices(self, i):
        data = []
        x = i * self.L3
        for z in [-self.s / 2, self.s / 2]:
            data = data + [
                (x, 0, z),
                (x + self.L1, 0, z),
                (x + self.L2, 0, z),
                (x + self.L3, 0, z),
                (x + self.L2, self.h, z),
                (x + self.L3, self.h, z),
                (x, self.H, z),
                (x + self.L1, self.H, z),
                (x + self.L2, self.H, z),
                (x + self.L3, self.H, z),
            ]
        return data

    def _create_vertices(self):
        vertices_array = []

        for i in range(self.n_steps):
            vertices_array = vertices_array + self._create_one_level_vertices(i)
        return vertices_array

    def _create_one_level_blocks(self, i):

        n = len(self._create_one_level_vertices(0))

        data = [
            "hex (%i %i %i %i %i %i %i %i) (%i %i %i) "
            % (
                i * n + 0,
                i * n + 1,
                i * n + 7,
                i * n + 6,
                i * n + 10,
                i * n + 11,
                i * n + 17,
                i * n + 16,
                self.mesh_den * self.L1,
                self.mesh_den * self.H,
                self.mesh_den * self.s,
            )
            + " simpleGrading ((1)((0.2 0.3 4)(0.6 0.4 1)(0.2 0.3 0.25))(1))",
            "hex (%i %i %i %i %i %i %i %i) (%i %i %i) "
            % (
                i * n + 1,
                i * n + 4,
                i * n + 8,
                i * n + 7,
                i * n + 11,
                i * n + 14,
                i * n + 18,
                i * n + 17,
                self.mesh_den * (self.L2 - self.L1),
                self.mesh_den * self.H,
                self.mesh_den * self.s,
            )
            + " simpleGrading ((1)((0.2 0.3 4)(0.6 0.4 1)(0.2 0.3 0.25))(1))",
            "hex (%i %i %i %i %i %i %i %i) (%i %i %i) "
            % (
                i * n + 2,
                i * n + 3,
                i * n + 5,
                i * n + 4,
                i * n + 12,
                i * n + 13,
                i * n + 15,
                i * n + 14,
                self.mesh_den * (self.L3 - self.L2),
                self.mesh_den * self.h,
                self.mesh_den * self.s,
            )
            + " simpleGrading (1 1 1)",
            "hex (%i %i %i %i %i %i %i %i) (%i %i %i) "
            % (
                i * n + 4,
                i * n + 5,
                i * n + 9,
                i * n + 8,
                i * n + 14,
                i * n + 15,
                i * n + 19,
                i * n + 18,
                self.mesh_den * (self.L3 - self.L2),
                self.mesh_den * (self.H),
                self.mesh_den * self.s,
            )
            + " simpleGrading ((1)((0.2 0.3 4)(0.6 0.4 1)(0.2 0.3 0.25))(1))",
        ]
        return data

    def _create_blocks(self):
        blocks_array = []

        for i in range(self.n_steps):
            blocks_array = blocks_array + self._create_one_level_blocks(i)
        return blocks_array

    def _create_edges(self):
        edges_array = []
        return edges_array

    def _create_one_level_patches(self, i):
        """Return a list of (name, type, [faces]) tuples for step i."""
        n = len(self._create_one_level_vertices(0))

        return [
            (
                "inlet%i" % i,
                "patch",
                ["(%i %i %i %i)" % (i * n + 0, i * n + 10, i * n + 16, i * n + 6)],
            ),
            (
                "outlet%i" % i,
                "patch",
                [
                    "(%i %i %i %i)" % (i * n + 5, i * n + 9, i * n + 19, i * n + 15),
                    "(%i %i %i %i)" % (i * n + 3, i * n + 5, i * n + 15, i * n + 13),
                ],
            ),
            (
                "lowerWall",
                "wall",
                [
                    "(%i %i %i %i)" % (i * n + 0, i * n + 1, i * n + 11, i * n + 10),
                    "(%i %i %i %i)" % (i * n + 1, i * n + 4, i * n + 14, i * n + 11),
                    "(%i %i %i %i)" % (i * n + 4, i * n + 2, i * n + 12, i * n + 14),
                    "(%i %i %i %i)" % (i * n + 2, i * n + 3, i * n + 13, i * n + 12),
                ],
            ),
            (
                "upperWall",
                "wall",
                [
                    "(%i %i %i %i)" % (i * n + 6, i * n + 16, i * n + 17, i * n + 7),
                    "(%i %i %i %i)" % (i * n + 8, i * n + 7, i * n + 17, i * n + 18),
                    "(%i %i %i %i)" % (i * n + 9, i * n + 8, i * n + 18, i * n + 19),
                ],
            ),
            (
                "back",
                "empty",
                [
                    "(%i %i %i %i)" % (i * n + 0, i * n + 6, i * n + 7, i * n + 1),
                    "(%i %i %i %i)" % (i * n + 1, i * n + 7, i * n + 8, i * n + 4),
                    "(%i %i %i %i)" % (i * n + 4, i * n + 8, i * n + 9, i * n + 5),
                    "(%i %i %i %i)" % (i * n + 2, i * n + 4, i * n + 5, i * n + 3),
                ],
            ),
            (
                "front",
                "empty",
                [
                    "(%i %i %i %i)" % (i * n + 10, i * n + 11, i * n + 17, i * n + 16),
                    "(%i %i %i %i)" % (i * n + 11, i * n + 14, i * n + 18, i * n + 17),
                    "(%i %i %i %i)" % (i * n + 14, i * n + 15, i * n + 19, i * n + 18),
                    "(%i %i %i %i)" % (i * n + 12, i * n + 13, i * n + 15, i * n + 14),
                ],
            ),
        ]

    def _create_patches(self):
        patch_order = []
        patch_types = {}
        patch_faces = {}
        for i in range(self.n_steps):
            for name, ptype, faces in self._create_one_level_patches(i):
                if name not in patch_faces:
                    patch_order.append(name)
                    patch_types[name] = ptype
                    patch_faces[name] = []
                patch_faces[name] += faces

        patches = []
        for name in patch_order:
            patches += [
                "    " + name,
                "    {",
                "        type %s;" % patch_types[name],
                "        faces",
                "        (",
            ]
            for f in patch_faces[name]:
                patches.append("            " + f)
            patches += ["        );", "    }"]
        return patches

    def _create_one_level_merge_patch_pairs(self, i):
        data = []
        if i < self.n_steps - 1:
            data.append("    ( outlet%i inlet%i )" % (i, i + 1))
        return data

    def _create_merge_patch_pairs(self):
        merge_patch_pairs_array = []
        for i in range(self.n_steps):
            merge_patch_pairs_array += self._create_one_level_merge_patch_pairs(i)
        return merge_patch_pairs_array

    def write_block_mesh_dict(self, filename="blockMeshDict"):
        vertices_data = self._create_vertices()
        entries = []
        entries.append("scale 1;\n")
        entries.append("vertices")
        entries.append("(")
        for index, one_vertex in enumerate(vertices_data):
            entries.append("    (%e %e %e) " % one_vertex + "// %i" % index)
        entries.append(");\n")

        blocks_data = self._create_blocks()
        entries.append("blocks")
        entries.append("(")
        for index, one_block in enumerate(blocks_data):
            entries.append("    " + one_block + "// %i" % index)
        entries.append(");\n")

        edges_data = self._create_edges()
        entries.append("edges")
        entries.append("(")
        for index, one_edge in enumerate(edges_data):
            entries.append("    " + one_edge + "// %i" % index)
        entries.append(");\n")

        boundary_data = self._create_patches()
        entries.append("boundary")
        entries.append("(")
        for index, one_patch in enumerate(boundary_data):
            entries.append(one_patch)
        entries.append(");\n")

        merge_patch_pairs_data = self._create_merge_patch_pairs()
        entries.append("mergePatchPairs")
        entries.append("(")
        for one_pair in merge_patch_pairs_data:
            entries.append(one_pair)
        entries.append(");")

        dirName = os.getcwd()
        writer = OpenFoamDataWriter(dirName, "", filename, entries)


if __name__ == "__main__":
    repeating_step = GenerateRepeatingStep()
    repeating_step.write_block_mesh_dict()