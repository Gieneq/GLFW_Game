from common.rect import Rect
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from map.mapdata import MapData
from chunking.chunk import Chunk
from common.config import Config

class Visualization2D:
    def __init__(self, config: Config):
        self.config = config

        self.bounding = None
        self.aligned_bounding = None

        self.chunks_briefs = []
        self.maps_briefs = []

    def set_boundings(self, bounding: Rect, aligned_bounding: Rect):
        self.bounding = bounding
        self.aligned_bounding = aligned_bounding

    def append_chunk(self, chunk: Chunk):
        chunks_brief = {
            "outline": Rect(chunk.x, chunk.y, chunk.width, chunk.height),
            "tilesets": chunk.tilesets_ids,
            "min_elevation_idx": chunk.min_elevation_idx,
            "max_elevation_idx": chunk.max_elevation_idx,
        }
        self.chunks_briefs.append(chunks_brief)

    def append_map_data(self, map_data: MapData):
        map_brief = {
            "outline": map_data.outline,
            "tilesets": map_data.tilesets_ids,
            "min_elevation_idx": map_data.min_elevation_idx,
            "max_elevation_idx": map_data.max_elevation_idx,
        }
        self.maps_briefs.append(map_brief)

    def show(self):
        pass
        if self.aligned_bounding is None or self.bounding is None:
            return

        # Create a figure and axis
        fig, ax = plt.subplots()

        """ Draw chunks """
        for chunk_brief in self.chunks_briefs:
            world_rect = chunk_brief["outline"]
            x = world_rect.x
            y = world_rect.y
            w = world_rect.width
            h = world_rect.height
            tilesets = ",".join((str(tileset_id) for tileset_id in chunk_brief["tilesets"]))
            elevations = f"Idx:{chunk_brief['max_elevation_idx']}-{chunk_brief['min_elevation_idx']}"

            draw_rect = patches.Rectangle((x, y), w, h, linewidth=1, edgecolor='r', facecolor='none')
            ax.add_patch(draw_rect)
            ax.text(x, y , tilesets, ha='left', va='top', fontsize='smaller', color='r',
                    bbox={'facecolor': 'white', 'linewidth': None, 'edgecolor': 'r', 'alpha': 0.0})
            ax.text(x + w, y + h , elevations, ha='right', va='bottom', fontsize='smaller', color='black',
                    bbox={'facecolor': 'white', 'linewidth': None, 'edgecolor': 'black', 'alpha': 0.0})

        """ Draw maps bounding """
        for maps_brief in self.maps_briefs:
            world_rect = maps_brief["outline"]
            map_bounding_rect = patches.Rectangle((world_rect.x, world_rect.y),
                                                      world_rect.width, world_rect.height,
                                                  fill=True,
                                                      linewidth=1, edgecolor='g', facecolor='g', alpha=0.6)
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
        ax.set_xlim(self.aligned_bounding.left() - self.config.chunk_width, self.aligned_bounding.right() + self.config.chunk_height)
        ax.set_ylim(self.aligned_bounding.top() - self.config.chunk_width, self.aligned_bounding.bottom() + self.config.chunk_height)

        plt.title(f"Maps merged and split into chunks of ({self.config.chunk_width} x {self.config.chunk_height})")

        plt.xlabel(f"World space X")
        plt.ylabel(f"World space Y")

        plt.gca().invert_yaxis()

        # Display the plot
        plt.show()

