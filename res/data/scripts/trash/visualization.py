from gcommon import Rect
import matplotlib.pyplot as plt
import matplotlib.patches as patches

class VisualizationData:
    def __init__(self, chunk_width, chunk_height):
        self.chunk_width = chunk_width
        self.chunk_height = chunk_height
        self._chunks = []
        self.bounding = None
        self.aligned_bounding = None
        self.maps_boundings = []

    def append_chunk(self, world_ret, tileset_ids, elevations_count=0, min_z=0, tiles_cout=0):
        self._chunks.append({"rect": world_ret, "tilesets": tileset_ids,
                             "elevations": elevations_count, "min_z": min_z, "tiles_cout": tiles_cout})

    def append_map_data(self, map_data):
        map_world_rect = Rect(map_data.x, map_data.y, map_data.width, map_data.height)
        self.maps_boundings.append(map_world_rect)

    def show(self):
        if self.aligned_bounding is None or self.bounding is None:
            return

        # Create a figure and axis
        fig, ax = plt.subplots()

        """ Draw chunks """
        for chunk in self._chunks:
            world_rect = chunk["rect"]
            x = world_rect.x
            y = world_rect.y
            w = world_rect.width
            h = world_rect.height
            tilesets = ",".join((str(tileset_id) for tileset_id in chunk["tilesets"]))
            elevations = f"{chunk['tiles_cout']}\nD={chunk['elevations']}\nZ={chunk['min_z']}"

            rect = patches.Rectangle((x, y), w, h, linewidth=1, edgecolor='r', facecolor='none')
            ax.add_patch(rect)
            ax.text(x, y , tilesets, ha='left', va='top', fontsize='smaller', color='r',
                    bbox={'facecolor': 'white', 'linewidth': None, 'edgecolor': 'r', 'alpha': 0.0})
            ax.text(x + w, y + h , elevations, ha='right', va='bottom', fontsize='smaller', color='black',
                    bbox={'facecolor': 'white', 'linewidth': None, 'edgecolor': 'black', 'alpha': 0.0})

        """ Draw maps bounding """
        for map_bounding in self.maps_boundings:
            map_bounding_rect = patches.Rectangle((map_bounding.x, map_bounding.y),
                                                      map_bounding.width, map_bounding.height,
                                                  fill=True,
                                                      linewidth=1, edgecolor='g', facecolor='g')
            ax.add_patch(map_bounding_rect)

        """ Draw world boundings """
        rect_bounding_aligned = patches.Rectangle((self.aligned_bounding.x, self.aligned_bounding.y),
                                                  self.aligned_bounding.width, self.aligned_bounding.height,
                                                  linewidth=1, edgecolor='b', facecolor='none')
        ax.add_patch(rect_bounding_aligned)

        rect_bounding = patches.Rectangle((self.bounding.x, self.bounding.y),
                                          self.bounding.width, self.bounding.height,
                                          linewidth=1, edgecolor='m', facecolor='none')
        ax.add_patch(rect_bounding)

        # Set axis limits based on the rectangles
        ax.set_xlim(self.aligned_bounding.left() - self.chunk_width, self.aligned_bounding.right() + self.chunk_height)
        ax.set_ylim(self.aligned_bounding.top() - self.chunk_width, self.aligned_bounding.bottom() + self.chunk_height)

        plt.title(f"Maps merged and split into chunks of ({self.chunk_width} x {self.chunk_height})")

        plt.xlabel(f"World space X")
        plt.ylabel(f"World space Y")

        plt.gca().invert_yaxis()

        # Display the plot
        plt.show()

