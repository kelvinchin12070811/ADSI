#! python3
########################################################################################################################
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
########################################################################################################################
import argparse
import imagehash
from PIL import Image

def hash_image(path):
    hash = str(imagehash.average_hash(Image.open(path))).upper()
    print(hash)

def compare_image(hash, path):
    hash = imagehash.hex_to_hash(hash)
    print(hash - imagehash.average_hash(Image.open(path)))

parser = argparse.ArgumentParser()
parser.add_argument('image_path', help = 'Path to image to hash')
parser.add_argument('--hash', help = 'Hash provided image', action = 'store_true')
parser.add_argument('--compare', help = 'Compare image with hash')
args = parser.parse_args()

if args.hash: hash_image(args.image_path)
if args.compare: compare_image(args.compare, args.image_path)