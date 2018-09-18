# Create a 4x4 2d woven textile with yarn spacing of 5 and thickness 2
# The fifth parameter indicates whether to refine the textile to avoid intersections
Textile = CTextileWeave2D(4, 4, 5, 2, False);

# Set the weave pattern
Textile.SwapPosition(3, 0);
Textile.SwapPosition(2, 1);
Textile.SwapPosition(1, 2);
Textile.SwapPosition(0, 3);

# Adjust the yarn width and height
Textile.SetYarnWidths(4);
Textile.SetYarnHeights(0.8);

# Setup a domain
Textile.AssignDefaultDomain()

# Add the textile
AddTextile(Textile)