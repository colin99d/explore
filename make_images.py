import pathlib
from PIL import Image

base_path = pathlib.Path(__file__).parent.resolve() / "images/"
resources = base_path / "base_images/"

backgrounds = ["discovered", "cleared"]

locations = ["", "house", "castle"]

# Allow for generation of spaces with no character
users = ["", "character"]


def crop_image(
    image: Image, left: float, upper: float, right: float, lower: float
) -> Image:
    "Crops a given image. Each number is the percentage you want to remove."
    width, height = image.size
    return image.crop(
        (width * left, upper * height, (1 - right) * width, (1 - lower) * height)
    )


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
    sword = load_image("sword")
    crop_image(sword, 0.3, 0.65, 0.5, 0).save(base_path / "sword.png")
    character = load_image("character")
    crop_image(character, 0.15, 0, 0.35, 0).save(base_path / "character.png")
    # Now generate images
    for back in backgrounds:
        for loc in locations:
            for use in users:
                create_image(back, loc, use)
