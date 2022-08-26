import pathlib
from PIL import Image

base_path = pathlib.Path(__file__).parent.resolve() / "images/"
resources = base_path / "base_images/"

backgrounds = ["discovered", "cleared"]

locations = ["", "house", "castle"]

# Allow for generation of spaces with no character
users = ["", "character"]


def load_image(ending: str) -> Image:
    if ending:
        return Image.open(resources / f"{ending}.png")
    return Image.new("RGBA", (400, 400), (255, 0, 0, 0))


def create_image(background: str, location: str, user: str):
    back_img = load_image(background)
    loc_img = load_image(location)
    usr_img = load_image(user)
    back_img.paste(loc_img, (0, 0), loc_img)
    back_img.paste(usr_img, (0, 0), usr_img)
    back_img.save(base_path / f"{background}-{location}-{user}.png")


if __name__ == "__main__":
    # First move over images that are not changed
    load_image("undiscovered").save(base_path / "undiscovered.png")
    # Now generate images
    for back in backgrounds:
        for loc in locations:
            for use in users:
                create_image(back, loc, use)
