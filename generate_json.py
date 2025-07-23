#!/usr/bin/env python3

import json
import random
import string
import sys
import os
from datetime import datetime

def random_string(length):
    """Generate a random string of given length."""
    return ''.join(random.choices(string.ascii_letters + string.digits + ' ', k=length))

def random_email():
    """Generate a random email address."""
    username = ''.join(random.choices(string.ascii_lowercase, k=random.randint(5, 12)))
    domain = ''.join(random.choices(string.ascii_lowercase, k=random.randint(4, 8)))
    tld = random.choice(['com', 'org', 'net', 'edu', 'gov'])
    return f"{username}@{domain}.{tld}"

def random_phone():
    """Generate a random phone number."""
    return f"+1-{random.randint(100, 999)}-{random.randint(100, 999)}-{random.randint(1000, 9999)}"

def generate_user():
    """Generate a random user object."""
    return {
        "id": random.randint(1, 1000000),
        "username": random_string(random.randint(6, 15)),
        "email": random_email(),
        "first_name": random_string(random.randint(4, 12)),
        "last_name": random_string(random.randint(4, 15)),
        "age": random.randint(18, 80),
        "phone": random_phone(),
        "is_active": random.choice([True, False]),
        "balance": round(random.uniform(0, 10000), 2),
        "tags": [random_string(random.randint(3, 8)) for _ in range(random.randint(1, 5))],
        "metadata": {
            "last_login": f"2024-{random.randint(1, 12):02d}-{random.randint(1, 28):02d}T{random.randint(0, 23):02d}:{random.randint(0, 59):02d}:{random.randint(0, 59):02d}Z",
            "login_count": random.randint(0, 500),
            "preferences": {
                "theme": random.choice(["dark", "light", "auto"]),
                "notifications": random.choice([True, False]),
                "language": random.choice(["en", "es", "fr", "de", "ja"])
            }
        }
    }

def generate_product():
    """Generate a random product object."""
    return {
        "id": f"prod_{random.randint(1000, 99999)}",
        "name": random_string(random.randint(10, 30)),
        "description": random_string(random.randint(50, 200)),
        "price": round(random.uniform(1, 1000), 2),
        "category": random.choice(["electronics", "clothing", "books", "home", "sports", "toys"]),
        "in_stock": random.randint(0, 100),
        "rating": round(random.uniform(1, 5), 1),
        "reviews": random.randint(0, 1000),
        "specifications": {
            "weight": f"{random.uniform(0.1, 50):.2f}kg",
            "dimensions": f"{random.randint(1, 100)}x{random.randint(1, 100)}x{random.randint(1, 100)}cm",
            "color": random.choice(["red", "blue", "green", "black", "white", "yellow", "purple"])
        }
    }

def generate_order():
    """Generate a random order object."""
    return {
        "order_id": f"order_{random.randint(10000, 999999)}",
        "user_id": random.randint(1, 1000000),
        "items": [
            {
                "product_id": f"prod_{random.randint(1000, 99999)}",
                "quantity": random.randint(1, 5),
                "price": round(random.uniform(10, 500), 2)
            } for _ in range(random.randint(1, 8))
        ],
        "total": round(random.uniform(20, 2000), 2),
        "status": random.choice(["pending", "processing", "shipped", "delivered", "cancelled"]),
        "created_at": f"2024-{random.randint(1, 12):02d}-{random.randint(1, 28):02d}T{random.randint(0, 23):02d}:{random.randint(0, 59):02d}:{random.randint(0, 59):02d}Z",
        "shipping_address": {
            "street": f"{random.randint(1, 9999)} {random_string(random.randint(5, 15))} St",
            "city": random_string(random.randint(6, 12)),
            "state": random_string(2).upper(),
            "zip_code": f"{random.randint(10000, 99999)}",
            "country": "US"
        }
    }

def generate_large_json(target_size_mb=100, output_file="benchmark.json"):
    """Generate a large JSON file with mixed data types."""
    target_size_bytes = target_size_mb * 1024 * 1024

    data = {
        "metadata": {
            "generated_at": datetime.now().isoformat(),
            "target_size_mb": target_size_mb,
            "generator_version": "1.0"
        },
        "users": [],
        "products": [],
        "orders": [],
        "analytics": {
            "page_views": [],
            "server_metrics": []
        }
    }

    print(f"Generating JSON file targeting {target_size_mb}MB...")

    # Keep track of approximate size
    current_size = 0
    batch_size = 100  # Generate in batches to avoid memory issues

    while current_size < target_size_bytes:
        # Add users
        for _ in range(batch_size):
            data["users"].append(generate_user())

        # Add products
        for _ in range(batch_size // 2):
            data["products"].append(generate_product())

        # Add orders
        for _ in range(batch_size // 3):
            data["orders"].append(generate_order())

        # Add some analytics data
        for _ in range(batch_size):
            data["analytics"]["page_views"].append({
                "timestamp": f"2024-{random.randint(1, 12):02d}-{random.randint(1, 28):02d}T{random.randint(0, 23):02d}:{random.randint(0, 59):02d}:{random.randint(0, 59):02d}Z",
                "page": f"/{random_string(random.randint(5, 15))}",
                "user_id": random.randint(1, 1000000),
                "duration": random.randint(1, 300)
            })

        for _ in range(batch_size // 10):
            data["analytics"]["server_metrics"].append({
                "timestamp": f"2024-{random.randint(1, 12):02d}-{random.randint(1, 28):02d}T{random.randint(0, 23):02d}:{random.randint(0, 59):02d}:{random.randint(0, 59):02d}Z",
                "cpu_usage": round(random.uniform(0, 100), 2),
                "memory_usage": round(random.uniform(0, 100), 2),
                "disk_io": random.randint(0, 1000),
                "network_io": random.randint(0, 10000)
            })

        # Estimate current size (rough approximation)
        current_size = (
            len(data["users"]) * 400 +  # Rough estimate per user
            len(data["products"]) * 300 +
            len(data["orders"]) * 500 +
            len(data["analytics"]["page_views"]) * 150 +
            len(data["analytics"]["server_metrics"]) * 200
        )

        if current_size % (10 * 1024 * 1024) < batch_size * 400:  # Print progress every ~10MB
            print(f"Generated approximately {current_size / (1024 * 1024):.1f}MB...")

    print(f"Writing to {output_file}...")
    with open(output_file, 'w') as f:
        json.dump(data, f, separators=(',', ':'))  # Compact format

    # Get actual file size
    actual_size = os.path.getsize(output_file)
    actual_size_mb = actual_size / (1024 * 1024)

    print(f"✓ Generated {output_file}")
    print(f"  Target size: {target_size_mb}MB")
    print(f"  Actual size: {actual_size_mb:.1f}MB ({actual_size:,} bytes)")
    print(f"  Users: {len(data['users']):,}")
    print(f"  Products: {len(data['products']):,}")
    print(f"  Orders: {len(data['orders']):,}")
    print(f"  Page views: {len(data['analytics']['page_views']):,}")
    print(f"  Server metrics: {len(data['analytics']['server_metrics']):,}")

def main():
    if len(sys.argv) > 1:
        try:
            target_size = int(sys.argv[1])
        except ValueError:
            print("Usage: python3 generate_json.py [size_in_mb] [output_file]")
            sys.exit(1)
    else:
        target_size = 100

    if len(sys.argv) > 2:
        output_file = sys.argv[2]
    else:
        output_file = f"benchmark_{target_size}mb.json"

    try:
        generate_large_json(target_size, output_file)
    except KeyboardInterrupt:
        print("\n\nInterrupted! Partial file may have been created.")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
